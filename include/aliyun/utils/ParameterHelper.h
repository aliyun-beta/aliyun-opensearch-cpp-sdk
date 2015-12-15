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

#ifndef ALIYUN_UTILS_PARAMETERHELPER_H_
#define ALIYUN_UTILS_PARAMETERHELPER_H_

#include <stddef.h>
#include <string>

#include "../Exception.h"
#include "Date.h"

namespace aliyun {
namespace utils {

class ParseDateException : public Exception {
 public:
  explicit ParseDateException(std::string msg)
      : Exception(msg) {
  }
};

class ParameterHelper {
 public:
  typedef unsigned char byte;
  typedef std::string string;

  static std::string md5hex(std::string str);

  static std::string md5Sum(std::string str);

  static byte * md5(const byte* data, size_t len);

  static string getUUID();

  static string getISO8601Date(Date date);

  static string getRFC2616Date(Date date);

  static Date parse(string strDate);

  static Date parseISO8601(string strDate);

  static Date parseRFC2616(string strDate);
};

}  // namespace utils
}  // namespace aliyun

#endif  // ALIYUN_UTILS_PARAMETERHELPER_H_
