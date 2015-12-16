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

#ifndef ALIYUN_HTTP_FORMAT_TYPE_H_
#define ALIYUN_HTTP_FORMAT_TYPE_H_

#include <string>

namespace aliyun {
namespace http {

class FormatType {
 public:
  enum Value {
    INVALID,
    XML,
    JSON,
    RAW
  };

  // implicit for convenience:
  //   cast from Value(Value => FormatType),   explicit can not
  //   assign from Value(FormatType <= Value), explicit need DIY
  FormatType(Value v = INVALID);

  const char* toString() const;

  // for compare
  inline operator int() const {
    return value_;
  }

  static std::string mapFormatToAccept(FormatType format);

  static FormatType mapAcceptToFormat(std::string accept);

 private:
  Value value_;
};

}  // namespace http
}  // namespace aliyun

#endif  // ALIYUN_HTTP_FORMAT_TYPE_H_
