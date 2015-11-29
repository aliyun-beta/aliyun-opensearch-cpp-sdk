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

#ifndef CORE_HTTP_PROTOCOLTYPE_H_
#define CORE_HTTP_PROTOCOLTYPE_H_

#include <string>

#include "../Exception.h"

namespace aliyun {

namespace http {

class ProtocolType {
 public:
  enum {
    INVALID,
    HTTP,
    HTTPS
  };

  ProtocolType(int v = 0)
      : value_(v) {
  }

  inline operator int() {
    return value_;
  }

  ProtocolType(std::string protocol);

  operator std::string();

  std::string toString();

 private:
  int value_;
};

}  // namespace http

}  // namespace aliyun

#endif  // CORE_HTTP_PROTOCOLTYPE_H_
