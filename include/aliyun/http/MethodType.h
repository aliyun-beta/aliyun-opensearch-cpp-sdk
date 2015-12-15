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

#ifndef ALIYUN_HTTP_METHODTYPE_H_
#define ALIYUN_HTTP_METHODTYPE_H_

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
  enum Value {
    INVALID,
    GET,
    PUT,
    POST,
    Delete,
    HEAD,
    OPTIONS
  };

  const char * toString() const;

  // implicit for convenience:
  //   cast from Value(Value => MethodType),   explicit can not
  //   assign from Value(MethodType <= Value), explicit need DIY
  MethodType(Value v = INVALID);

  explicit MethodType(std::string method);

  inline operator int() {
    return value_;
  }

 private:
  static const char** valueNames() {
    static const char *names[] = {
#define S(e) #e
      S(INVALID), S(GET), S(PUT), S(POST), S(Delete), S(HEAD), S(OPTIONS)
#undef S
    };
    return names;
  }

  Value value_;
};

}  // namespace http
}  // namespace aliyun

#endif  // ALIYUN_HTTP_METHODTYPE_H_
