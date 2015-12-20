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

#ifndef ALIYUN_UTILS_STRING_UTILS_H_
#define ALIYUN_UTILS_STRING_UTILS_H_

#include <sstream>
#include <string>

#define BEGIN_ALIYUN_TOSTRING_DECLARE \
namespace aliyun { \
namespace utils {  \
namespace StringUtils {

#define END_ALIYUN_TOSTRING_DECLARE \
}  /* StringUtils */ \
}  /* utils */       \
}  /* aliyun */

namespace aliyun {
namespace utils {
namespace StringUtils {  // string utility functions

std::string ToLowerCase(std::string str);

std::string ToUpperCase(std::string str);

template<typename T>
std::string ToString(T t) {
  std::stringstream ss;
  ss << t;
  return ss.str();
}

// TODO(xu): string encoding convert
std::string ToEncoding(std::string src, std::string encoding);

std::string hexString(std::string src, bool caps = true);

std::string hexDump(void* ptr, int len, bool caps = true);

std::string trim(std::string src);

}  // namespace StringUtils
}  // namespace utils
}  // namespace aliyun

#endif  // ALIYUN_UTILS_STRING_UTILS_H_
