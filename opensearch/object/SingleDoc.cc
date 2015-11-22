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

#include "SingleDoc.h"

#include "../CloudsearchDoc.h"

namespace aliyun {
namespace opensearch {

SingleDoc::SingleDoc() {
}

SingleDoc::SingleDoc(string cmd, const std::map<string, string>& fields)
    : command_(cmd),
      fields_(fields) {
}

void SingleDoc::addField(string key, string value) {
  if (value.find(CloudsearchDoc::HA_DOC_MULTI_VALUE_SEPARATOR)) {
    string::size_type start = 0;
    string::size_type pos = value.find(
        CloudsearchDoc::HA_DOC_MULTI_VALUE_SEPARATOR);

    string jsonArray = "[";
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
  string json = "{cmd:" + command_;

  string jsonFields = "{";
  for (std::map<string, string>::const_iterator it = fields_.begin();
      it != fields_.end(); ++it) {
    jsonFields += it->first + ':' + it->second + ',';
  }

  if (jsonFields.length() > 1) {
    jsonFields[jsonFields.length() - 1] = '}';
    json += ',' + jsonFields + '}';
  } else {
    jsonFields.resize(0);
    json += '}';
  }

  return json;
}

} /* namespace opensearch */
} /* namespace aliyun */

