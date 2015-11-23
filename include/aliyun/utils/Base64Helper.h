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

#ifndef CORE_UTILS_BASE64HELPER_H_
#define CORE_UTILS_BASE64HELPER_H_

#include <string>
#include <vector>

#include "StringUtils.h"

namespace aliyun {

namespace utils {

class Base64Helper {
 public:

  typedef unsigned char byte;

  static std::string encode(std::vector<byte>& buffer);

  static std::string encode(const byte* buffer, int length);

  static std::string encode(std::string str, std::string encoding);

  static std::string decode(std::string str, std::string encoding);

 private:
  static const char BASE64_CODE[];

  static const int BASE64_DECODE[];
};

}  // namespace utils

}  // namespace aliyun

#endif  // CORE_UTILS_BASE64HELPER_H_
