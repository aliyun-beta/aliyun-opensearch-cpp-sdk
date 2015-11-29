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

#include "aliyun/http/ProtocolType.h"

namespace aliyun {

namespace http {

ProtocolType::ProtocolType(std::string protocol) {
  if (strncasecmp(protocol.c_str(), "http", sizeof("http")) == 0) {
    value_ = HTTP;
  } else if (strncasecmp(protocol.c_str(), "https", sizeof("https")) == 0) {
    value_ = HTTPS;
  } else
    throw Exception("Unknow ProtocolType Names");
}

std::string ProtocolType::toString() {
  static std::string enumNames[] = { "invalid", "http", "https" };
  int v = value_;
  return enumNames[INVALID <= v && v <= HTTPS ? v : 0];
}

ProtocolType::operator std::string() {
  return toString();
}
}  // namespace http

}  // namespace aliyun
