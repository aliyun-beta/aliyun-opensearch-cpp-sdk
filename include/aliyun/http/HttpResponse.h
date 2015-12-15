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

#ifndef ALIYUN_HTTP_HTTPRESPONSE_H_
#define ALIYUN_HTTP_HTTPRESPONSE_H_

#include <string>

#include "FormatType.h"
#include "HttpRequest.h"

namespace aliyun {
namespace http {

class HttpResponse : public HttpRequest {
 public:
  typedef std::string string;
  typedef http::FormatType FormatType;

  HttpResponse() {
    status_ = 0;
  }

  explicit HttpResponse(std::string url)
      : HttpRequest(url) {
    status_ = 0;
  }

  // override
  void setContent(string content, string encoding, FormatType format) {
    content_ = content;
    encoding_ = encoding;
    contentType_ = format;
  }

  std::string getHeaderValue(std::string name) const;

  static HttpResponse getResponse(HttpRequest request);

  int getStatus() const {
    return status_;
  }

  void setStatus(int status) {
    status_ = status;
  }

  bool isSuccess() const {
    if (200 <= status_ && 300 > status_)
      return true;
    return false;
  }

 private:
  static void parseParameters(HttpResponse* response);

 private:
  int status_;
};

}  // namespace http
}  // namespace aliyun

#endif  // ALIYUN_HTTP_HTTPRESPONSE_H_
