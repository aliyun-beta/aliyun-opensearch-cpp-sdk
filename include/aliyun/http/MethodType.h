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

#ifndef CORE_HTTP_METHODTYPE_H_
#define CORE_HTTP_METHODTYPE_H_

#include <string.h>
#include <string>

namespace aliyun {

namespace http {

/**
 * for HTTP request methods
 *
 * HTTP defines methods (sometimes referred to as verbs) to indicate the
 * desired action to be performed on the identified resource. What this
 * resource represents, whether pre-existing data or data that is generated
 * dynamically, depends on the implementation of the server.
 *
 * Often, the resource corresponds to a file or the output of an executable
 * residing on the server. The HTTP/1.0 specification defined the GET,
 * POST and HEAD methods and the HTTP/1.1 specification added 5 new methods:
 * OPTIONS, PUT, DELETE, TRACE and CONNECT.
 * https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol#Request_methods
 * */
class MethodType {
 public:
  enum {
    INVALID,
    GET,
    PUT,
    POST,
    DELETE,
    HEAD,
    OPTIONS
  };

  static const int kNumNames;

  const char * toString() const;

  MethodType(int v = 0);

  MethodType(std::string method);

  inline operator int() {
    return value_;
  }

 private:
  int value_;
};

}  // namespace http

}  // namespace aliyun

#endif  // CORE_HTTP_METHODTYPE_H_
