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

#include "aliyun/opensearch/object/SchemaTableFieldType.h"

namespace aliyun {
namespace opensearch {
namespace object {

SchemaTableFieldType::SchemaTableFieldType(int v)
    : value_(0 <= v && v <= kMaxValue ? v : 0) {
}

SchemaTableFieldType::SchemaTableFieldType(std::string type,
                                           std::string bigType) {
  for (value_ = 1; value_ <= kMaxValue; value_++) {
    if (::strncasecmp(type.c_str(), getTypeName(), type.length()) == 0
        && ::strncasecmp(bigType.c_str(), getBigTypeName(), bigType.length())
            == 0) {
      return;
    }
  }
  value_ = 0;
}

const char* SchemaTableFieldType::getTypeName() const {
  const char** names = typeNames();
  return names[value_];
}

}  // namespace object
}  // namespace opensearch
}  // namespace aliyun
