/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <fstream>
#ifdef _MSC_VER
#include <windows.h>
#else  //  _MSC_VER
#include <unistd.h>
#endif  //  _MSC_VER

#include "aliyun/utils/string_utils.h"
#include "aliyun/opensearch/cloudsearch_doc.h"
#include "aliyun/opensearch/cloudsearch_client.h"
#include "aliyun/opensearch/object/single_doc.h"
#include "aliyun/opensearch/object/doc_items.h"

namespace aliyun {
namespace opensearch {

using std::string;
using aliyun::auth::AcsURLEncoder;
using utils::StringUtils::ToString;

const string CloudsearchDoc::DOC_ADD = "add";
const string CloudsearchDoc::DOC_REMOVE = "delete";
const string CloudsearchDoc::DOC_UPDATE = "update";

const string CloudsearchDoc::PUSH_RETURN_STATUS_OK = "OK";

const string CloudsearchDoc::HA_DOC_ITEM_SEPARATOR = "\x1E";
const string CloudsearchDoc::HA_DOC_FIELD_SEPARATOR = "\x1F";
const string CloudsearchDoc::HA_DOC_MULTI_VALUE_SEPARATOR = "\x1D";
const string CloudsearchDoc::HA_DOC_SECTION_WEIGHT = "\x1C";

CloudsearchDoc::CloudsearchDoc(string indexName, ClientRef client) {
  this->indexName_ = indexName;
  this->client_ = &client;
  this->path_ = "/index/doc/" + this->indexName_;
}

string CloudsearchDoc::detail(string docId) {
  std::map<string, string> params;
  params["id"] = docId;
  return client_->call(this->path_, params, CloudsearchClient::METHOD_POST,
                       this->debugInfo_);
}

static bool isNotBlank(string str) {
  if (str.length() != 0 && utils::StringUtils::trim(str).length() != 0) {
    return true;
  }
  return false;
}

void CloudsearchDoc::operate(string cmd,
                             const std::map<string, string>& fields) {
  object::SingleDoc doc(cmd, fields);
  requestArray_.push_back(doc.getJsonString());
}

void CloudsearchDoc::add(const std::map<string, string>& fields) {
  operate("add", fields);
}

void CloudsearchDoc::update(const std::map<string, string>& fields) {
  operate("update", fields);
}

void CloudsearchDoc::remove(const std::map<string, string>& fields) {
  operate("delete", fields);
}

static string toJsonArray(const std::vector<string>& vec) {
  string jsonArray = "[";
  for (size_t i = 0; i < vec.size(); i++) {
    jsonArray += vec[i] + ',';
  }
  if (jsonArray.length() > 1) {
    jsonArray.resize(jsonArray.length() - 1);
  }
  jsonArray += ']';
  return jsonArray;
}

string CloudsearchDoc::push(string tableName) {
  std::map<string, string> params;

  params["action"] = "push";
  params["items"] = toJsonArray(this->requestArray_);
  params["table_name"] = tableName;
  params["sign_mode"] = utils::StringUtils::ToString(SIGN_MODE);

  string result = this->client_->call(this->path_, params,
                                      CloudsearchClient::METHOD_POST,
                                      this->debugInfo_);
  this->debugInfo_ += params["items"];
  this->requestArray_.clear();
  return result;
}

string CloudsearchDoc::push(string docs, string tableName) {
  std::map<string, string> params;

  params["action"] = "push";
  params["items"] = docs;
  params["table_name"] = tableName;
  params["sign_mode"] = utils::StringUtils::ToString(SIGN_MODE);

  return this->client_->call(this->path_, params,
                             CloudsearchClient::METHOD_POST, this->debugInfo_);
}

string CloudsearchDoc::pushHADocFile(string filePath, string tableName) {
  return pushHADocFile(filePath, tableName, 0);
}

string CloudsearchDoc::pushHADocFile(string filePath, string tableName,
                                     int64_t offset) {
  object::DocItems docItems;
  object::SingleDoc singleDoc;

  string lastField;
  string lastKey;
  uint64_t totalSize = 0;

  std::queue<time_t> timeLimitQueue;
  timeLimitQueue.push(::time(NULL));

  string result;

  std::ifstream input(filePath);
  string line;
  int64_t lineNumber = 1;  // line number
  int lastLineNumber = 0;  // last successfully pushed line number

  while (input >> line) {
    if (lineNumber < offset) {
      lineNumber++;
      continue;
    }

    string separator = line.substr(line.length() - 1);

    if (separator == HA_DOC_ITEM_SEPARATOR) {  // foreach doc
      lastField = "";
      string jstr = singleDoc.getJsonString();
      int currentSize = auth::AcsURLEncoder::encode(jstr).length();
      // FIXME(xu): why not += ?
      totalSize = AcsURLEncoder::encode(docItems.getJsonString()).length();
      if (currentSize + totalSize >= PUSH_MAX_SIZE) {
        timeLimitCheck(&timeLimitQueue);
        result = this->push(docItems.getJsonString(), tableName);
        string status = "status";
        string::size_type pos = result.find(status);
        if (pos != string::npos
            && result.substr(pos + status.length() + 1).find("OK") != 0) {
          return "last push not OK, line "
              + utils::StringUtils::ToString(lastLineNumber);
        }
        lastLineNumber = lineNumber;
        docItems = object::DocItems();
      }
      docItems.addDoc(singleDoc);
      singleDoc = object::SingleDoc();
    } else if (separator == HA_DOC_FIELD_SEPARATOR) {  // foreach field
      string detail = line.substr(0, line.length() - 1);
      if (isNotBlank(lastField)) {
        singleDoc.addField(lastKey, lastField + detail);
      } else {
        string::size_type middleIndex = detail.find("=");
        if (middleIndex != string::npos) {
          string key = detail.substr(0, middleIndex);
          string val = detail.substr(middleIndex + 1);
          if (key == "CMD") {
            singleDoc.setCommand(val);
          } else {
            singleDoc.addField(key, val);
          }
        }
      }

      lastField = "";
    } else {
      if (isNotBlank(lastField)) {
        lastField += lastField + line;
      }
    }

    lineNumber++;
  }
  result = this->push(docItems.getJsonString(), tableName);
  return result;
}

void CloudsearchDoc::timeLimitCheck(std::queue<time_t>* timeLimitQueue) {
  if (timeLimitQueue->size() < PUSH_FREQUENCE) {
    timeLimitQueue->push(::time(NULL));
  } else {
    time_t firstTime = timeLimitQueue->front();
    timeLimitQueue->pop();
    time_t currentTime = ::time(NULL);
    time_t delta = currentTime - firstTime;
    if (delta < 1000) {  // sleep a duration
#ifdef _MSC_VER
      ::Sleep(delta);
#else  // _MSC_VER
      ::usleep(delta * 1000);
#endif  // _MSC_VER
    }
  }
}

}  // namespace opensearch
}  // namespace aliyun
