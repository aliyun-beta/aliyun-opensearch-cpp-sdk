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

#include "aliyun/opensearch/CloudsearchIndex.h"

namespace aliyun {
namespace opensearch {

using std::map;
using std::string;
using utils::StringUtils::ToString;

CloudsearchIndex::CloudsearchIndex(std::string indexName, ClientRef client) {
  this->indexName_ = indexName;
  this->client_ = &client;
  this->path_ = "/index/" + this->indexName_;
}

std::string CloudsearchIndex::createByTemplateName(
    std::string templateName, const std::map<std::string, std::string> &opts) {
  map<string, string> params;
  params["action"] = "create";
  params["template"] = templateName;

  map<string, string>::const_iterator pos = opts.find("desc");
  if (opts.size() != 0 && pos != opts.end()) {
    params["index_desc"] = pos->second;
  }

  pos = opts.find("package_id");
  if (opts.size() != 0 && pos != opts.end()) {
    params["package_id"] = pos->second;
  }

  return this->client_->call(this->path_, params,
                             CloudsearchClient::METHOD_GET, this->debugInfo_);
}


std::string CloudsearchIndex::createByTemplateName(std::string templateName) {
  map<string, string> emptyMap;
  return createByTemplateName(templateName, emptyMap);
}

std::string CloudsearchIndex::rename(
    std::string toIndexName, const std::map<std::string, std::string> &opts) {
  map<string, string> params;

  params["action"] = "update";
  params["new_index_name"] = toIndexName;

  map<string, string>::const_iterator pos = opts.find("desc");
  if (opts.size() != 0) {
    params["description"] = pos->second;
  }

  string result = this->client_->call(this->path_,
                                      params,
                                      CloudsearchClient::METHOD_GET,
                                      this->debugInfo_);

  // TODO(xu): update this.indexName and this.path
  if (result.size() != 0 && result.find("status:OK") != string::npos) {
    this->indexName_ = toIndexName;
    this->path_ = "/index/" + this->indexName_;
  }

  return result;
}

std::string CloudsearchIndex::remove() {
  std::map<std::string, std::string> params;
  params["action"] = "delete";
  return this->client_->call(this->path_, params,
                             CloudsearchClient::METHOD_GET, this->debugInfo_);
}

std::string CloudsearchIndex::status() {
  std::map<string, string> params;
  params["action"] = "status";
  return this->client_->call(this->path_, params,
                             CloudsearchClient::METHOD_GET, this->debugInfo_);
}


std::string CloudsearchIndex::listIndexes(int page, int pageSize) {
  std::map<string, string> params;

  params["page"] = page == 0 ? "1" : ToString(page);
  params["page_size"] = pageSize == 0 ? "10" : ToString(pageSize);

  return this->client_->call("/index", params,
                             CloudsearchClient::METHOD_GET, this->debugInfo_);
}

std::string CloudsearchIndex::createTask(std::string operate,
                                         std::string tableName,
                                         bool needBuild) {
  std::map<string, string> params;
  params["action"] = "createTask";
  params["operate"] = operate;
  if (tableName.length() != 0) {
    params["table_name"] = tableName;
  } else {
    params["table_name"] = "";
  }
  params["need_build"] = needBuild ? "1" : "0";

  return this->client_->call(this->path_, params,
                             CloudsearchClient::METHOD_GET, this->debugInfo_);
}

std::string CloudsearchIndex::createBuildTask() {
  return createTask("build", "", false);
}

std::string CloudsearchIndex::createImportTask(std::string tableName,
                                               bool needBuild) {
  return createTask("import", tableName, needBuild);
}

std::string CloudsearchIndex::getErrorMessage(int page, int pageSize) {
  std::map<string, string> params;

  params["page"] = ToString(page);
  params["page_size"] = ToString(pageSize);

  return this->client_->call("/index/error" + this->indexName_,
                             params,
                             CloudsearchClient::METHOD_GET,
                             this->debugInfo_);
}

}  // namespace opensearch
}  // namespace aliyun
