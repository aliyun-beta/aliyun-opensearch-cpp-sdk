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

#ifndef CORE_UTILS_STRINGUTILS_H_
#define CORE_UTILS_STRINGUTILS_H_

#include <sstream>
#include <string>

namespace aliyun {

namespace utils {

// string utility functions
namespace StringUtils {

std::string ToLowerCase(std::string str);

std::string ToUpperCase(std::string str);

template<typename T>
std::string ToString(T t) {
  std::stringstream ss;
  ss << t;
  return ss.str();
}

// TODO: string encoding convert
std::string ToEncoding(std::string src, std::string encoding);

std::string hexString(std::string src, bool caps = true);

std::string hexDump(void* ptr, int len, bool caps = true);

std::string trim(std::string src);

}  // namespace StringUtils

}  // namespace utils

}  // namespace aliyun

#endif  // CORE_UTILS_STRINGUTILS_H_
