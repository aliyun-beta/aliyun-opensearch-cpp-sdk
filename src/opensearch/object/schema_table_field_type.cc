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
#include <apr_general.h>

#include "aliyun/opensearch/object/schema_table_field_type.h"

namespace aliyun {
namespace opensearch {
namespace object {

SchemaTableFieldType::SchemaTableFieldType(Value v)
    : value_(v) {
}

SchemaTableFieldType::SchemaTableFieldType(std::string type,
                                           std::string bigType) {
  for (int i = 1; i <= kMaxValue; i++) {
    if (strncasecmp(type.c_str(), typeNames()[i], type.length()) == 0
        && strncasecmp(bigType.c_str(), bigTypeNames()[i], bigType.length())
            == 0) {
      value_ = Value(i);
      return;
    }
  }
  value_ = INVALID;
}

std::string SchemaTableFieldType::getTypeName() const {
  return typeNames()[value_];
}

}  // namespace object
}  // namespace opensearch
}  // namespace aliyun
