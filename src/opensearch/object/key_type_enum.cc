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

#include "aliyun/opensearch/object/key_type_enum.h"

#include <apr_general.h>
#include <string.h>

namespace aliyun {
namespace opensearch {
namespace object {

KeyTypeEnum::KeyTypeEnum(Value v)
    : value_(v) {
}

KeyTypeEnum::KeyTypeEnum(std::string str) {
  for (int i = 1; i <= kMaxValue; i++) {
    if (strncasecmp(str.c_str(), valueNames()[i], str.length()) == 0) {
      value_ = Value(i);
      return;
    }
  }
  value_ = INVALID;
}

std::string KeyTypeEnum::toString() {
  return valueNames()[value_];
}

}  // namespace object
}  // namespace opensearch
}  // namespace aliyun
