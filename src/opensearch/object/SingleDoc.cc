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

#include "aliyun/opensearch/object/SingleDoc.h"
#include "aliyun/opensearch/CloudsearchDoc.h"

namespace aliyun {
namespace opensearch {
namespace object {

SingleDoc::SingleDoc() {
}

SingleDoc::SingleDoc(string cmd, const std::map<string, string> &fields)
    : command_(cmd),
      fields_(fields) {
}

void SingleDoc::addField(string key, string value) {
  string jsonArray = "[";
  string::size_type start = 0;
  string::size_type pos = value.find(CloudsearchDoc::HA_DOC_MULTI_VALUE_SEPARATOR);

  if (pos != string::npos) {
    while (pos != string::npos) {
      jsonArray += value.substr(start, pos - start) + ",";
      start = pos + 1;
      pos = value.find(CloudsearchDoc::HA_DOC_MULTI_VALUE_SEPARATOR, pos + 1);
    }
    jsonArray += value.substr(start);  // rest part
    jsonArray += ']';
    this->fields_[key] = jsonArray;
  } else {
    this->fields_[key] = value;
  }
}

std::string SingleDoc::getJsonString() const {
  string json = "{";

  // build command
  if (command_.length() > 0) {
    json += "cmd:" + command_;
  }

  // build fields
  string jsonFields = "{";
  for (std::map<string, string>::const_iterator it = fields_.begin();
       it != fields_.end(); ++it) {
    jsonFields += it->first + ':' + it->second + ',';
  }
  if (jsonFields.length() > 1) {
    jsonFields[jsonFields.length() - 1] = '}';
  } else {
    jsonFields.clear();
  }

  // concat two part
  if (json.length() > 1) {
    if (jsonFields.length() != 0) {
      json += ',' + jsonFields;
    }
    json += '}';
  }
  else {
    json.clear();
  }
  return json;
}

}  // namespace object
}  // namespace opensearch
}  // namespace aliyun

