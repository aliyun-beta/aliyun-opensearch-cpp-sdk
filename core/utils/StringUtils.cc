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

#include <ctype.h>
#include <string>

#include "StringUtils.h"

namespace aliyun {

namespace utils {

// string utility functions
namespace StringUtils {

std::string ToLowerCase(std::string str) {
  std::string lowered(str);
  for (size_t i = 0; i < str.length(); i++) {
    if (::isalpha(lowered[i]))
      lowered[i] = (char) ::tolower((int) str[i]);
  }
  return lowered;
}

std::string ToUpperCase(std::string str) {
  std::string lowered(str);
  for (size_t i = 0; i < str.length(); i++) {
    if (::isalpha(lowered[i]))
      lowered[i] = (char) ::toupper((int) str[i]);
  }
  return lowered;
}

// TODO: string encoding convert
std::string ToEncoding(std::string src, std::string encoding) {
  return src;
}

std::string hexString(std::string src, bool caps) {
  std::string result;
  const char* HEX = caps ? "0123456789ABCDEF" : "0123456789abcdef";
  for (size_t i = 0; i < src.length(); i++) {
    result.push_back(HEX[(src[i] >> 4) & 0x0F]);
    result.push_back(HEX[src[i] & 0x0F]);
  }
  return result;
}

std::string hexDump(void* ptr, int len, bool caps) {
  std::string result;
  char* cptr = static_cast<char*>(ptr);
  const char* HEX = caps ? "0123456789ABCDEF" : "0123456789abcdef";
  for (int i = 0; i < len; i++) {
    result.push_back(HEX[(cptr[i] >> 4) & 0x0F]);
    result.push_back(HEX[cptr[i] & 0x0F]);
  }
  return result;
}


std::string trim(std::string src) {
  std::string dst = src;
  for (size_t i = 0; i < dst.length(); i++) {
    if (!::isspace(dst[i])) {
      dst = dst.substr(i); // trim head spaces.
      break;
    }
  }

  for (int i = dst.length() - 1; i >= 0; i--) {
    if (!::isspace(dst[i])) {
      dst.resize(i + 1); // trim tail spaces.
      break;
    }
  }
  return dst;
}

}  // namespace StringUtils

}  // namespace utils

}  // namespace aliyun

