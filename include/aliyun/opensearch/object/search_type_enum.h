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

#ifndef ALIYUN_OPENSEARCH_OBJECT_SEARCH_TYPE_ENUM_H_
#define ALIYUN_OPENSEARCH_OBJECT_SEARCH_TYPE_ENUM_H_

#include <string>

namespace aliyun {
namespace opensearch {
namespace object {

class SearchTypeEnum {
 public:
  enum Value {
    INVALID,
    SEARCH,
    SCROLL
  };
  static const int kMaxValue = SCROLL;

  // implicit for convenience:
  //   cast from Value(Value => KeyTypeEnum),   explicit can not
  //   assign from Value(KeyTypeEnum <= Value), explicit need DIY
  SearchTypeEnum(Value v = INVALID);

  explicit SearchTypeEnum(std::string str);

#define IMPLEMENT_COMPARE(op) \
  bool operator op (const SearchTypeEnum& rhs) const { \
    return this->value_ op rhs.value_; \
  }

  IMPLEMENT_COMPARE(==)

#undef IMPLEMENT_COMPARE

  std::string toString();

 private:
  static const char** valueNames() {
    static const char* names[] = {
#define S(v) #v
        S(INVALID), S(SEARCH), S(SCROLL)
#undef S
        };
    return names;
  }

 private:
  Value value_;
};

}  // namespace object
}  // namespace opensearch
}  // namespace aliyun

#endif  // ALIYUN_OPENSEARCH_OBJECT_SEARCH_TYPE_ENUM_H_
