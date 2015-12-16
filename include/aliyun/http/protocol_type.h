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

#ifndef ALIYUN_HTTP_PROTOCOL_TYPE_H_
#define ALIYUN_HTTP_PROTOCOL_TYPE_H_

#include <string>

#include "../exception.h"

namespace aliyun {
namespace http {

class ProtocolType {
 public:
  enum Value {
    INVALID,
    HTTP,
    HTTPS
  };

  // implicit for convenience:
  //   cast from Value(Value => ProtocolType),   explicit can not
  //   assign from Value(ProtocolType <= Value), explicit need DIY
  ProtocolType(Value v = INVALID)
      : value_(v) {
  }

  inline operator int() {
    return value_;
  }

  explicit ProtocolType(std::string protocol);

  std::string toString();

 private:
  Value value_;
};

}  // namespace http
}  // namespace aliyun

#endif  // ALIYUN_HTTP_PROTOCOL_TYPE_H_
