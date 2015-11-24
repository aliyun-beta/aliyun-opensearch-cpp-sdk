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

#include "aliyun/opensearch/object/SchemaTableField.h"
#include "aliyun/utils/StringUtils.h"

namespace aliyun {
namespace opensearch {
namespace object {

SchemaTableField::SchemaTableField() {
  this->display_ = true;
  this->primarykey_ = false;
  this->multi_ = false;
  this->filter_ = false;
  this->search_ = false;
  this->aggregate_ = false;
}

void SchemaTableField::addIndex(std::string indexStr) {
  std::string trim = utils::StringUtils::trim(indexStr);

  if (indexStr.length() == 0 || trim.length() == 0) {
    return;
  }
  for (size_t i = 0; i < indexList_.size(); i++) {
    if (indexList_[i] == trim) {
      return;
    }
  }
  indexList_.push_back(trim);
}

}  // namespace object
}  // namespace opensearch
}  // namespace aliyun
