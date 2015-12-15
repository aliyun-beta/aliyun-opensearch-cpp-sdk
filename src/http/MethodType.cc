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
#include "aliyun/http/MethodType.h"

namespace aliyun {
namespace http {

const char *MethodType::toString() const {
  int v = value_;
  const char* NAMES[] = {
#define S(e) #e
      S(INVALID), S(GET), S(PUT), S(POST), S(Delete), S(HEAD), S(OPTIONS)
#undef S
      };
  return NAMES[(0 <= v && v <= OPTIONS) ? v : INVALID];
}

MethodType::MethodType(Value v)
    : value_(v) {
}

MethodType::MethodType(std::string method) {
  for (int i = GET; i <= OPTIONS; i++) {
    if (::strncasecmp(method.c_str(), valueNames()[i], method.length()) == 0) {
      value_ = Value(i);
      return;
    }
  }
  value_ = INVALID;
}

#if 0
MethodType &MethodType::operator=(MethodType::Value v) {
  this->value_ = v;
  return *this;
}
#endif

}  // namespace http
}  // namespace aliyun

