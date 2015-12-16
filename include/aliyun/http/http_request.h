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

#ifndef ALIYUN_HTTP_HTTP_REQUEST_H_
#define ALIYUN_HTTP_HTTP_REQUEST_H_

#include <curl/curl.h>
#include <map>
#include <string>

#include "aliyun/exception.h"
#include "aliyun/http/format_type.h"
#include "aliyun/http/method_type.h"

namespace aliyun {
namespace http {

class CurlException : public Exception {
 public:
  explicit CurlException(CURLcode rc)
      : Exception(curl_easy_strerror(rc)) {
  }

  explicit CurlException(std::string what)
      : Exception(what) {
  }
};

// guard CURL handle for exception throws
class CurlHandle {
 public:
  CurlHandle();

  CurlHandle(const CurlHandle& rhs);

  ~CurlHandle();

  // convert to CURL*
  inline operator CURL*() {
    return curl_;
  }

  curl_slist* head_;
  char* post_;

 private:
  // disallow common assignment.
  CurlHandle& operator=(const CurlHandle& rhs);

  int* refs_;
  CURL* curl_;
};

class HttpRequest {
 public:
  HttpRequest();

  explicit HttpRequest(std::string url);

  HttpRequest(std::string url,
              const std::map<std::string, std::string>& headers);

  // getters and setters
  std::string getUrl() const {
    return url_;
  }

  void setUrl(std::string url) {
    url_ = url;
  }

  const std::string& getEncoding() const {
    return encoding_;
  }

  void setEncoding(const std::string encoding) {
    encoding_ = encoding;
  }

  FormatType getContentType() const {
    return contentType_;
  }

  void setContentType(FormatType contentType);

  MethodType getMethod() const {
    return method_;
  }

  void setMethod(MethodType method) {
    method_ = method;
  }

  void setMethod(std::string method) {
    method_ = MethodType(method);
  }

  std::string getContent() const {
    return content_;
  }

  // modifiable
  std::string& content() {
    return content_;
  }

  std::string getHeaderValue(std::string name) const;

  void putHeaderParameter(std::string name, std::string value);

  void removeHeaderParameter(std::string name);

  void setContent(std::string content, std::string encoding, FormatType format);

  const std::map<std::string, std::string>& getHeaders() const {
    return headers_;
  }

  CurlHandle getHttpConnection();

  std::string getContentTypeValue(FormatType contentType, std::string encoding);

  static void setSSLVerifyHost(int option) {
    sSSLVerifyHost = option;
  }

  static void setSSLVerifyPeer(int option) {
    sSSLVerifyPeer = option;
  }

  static long getSslVerifyHost() {
    return sSSLVerifyHost;
  }

  static long getSslVerifyPeer() {
    return sSSLVerifyPeer;
  }

  static void enableGzip(bool enable);

 protected:
  std::string url_;
  MethodType method_;
  FormatType contentType_;
  std::string content_;
  std::string encoding_;
  std::map<std::string, std::string> headers_;

 private:
  // determines whether verifies the authenticity of the peer's certificate.
  static long sSSLVerifyPeer;  // long: follow libcurl

  // determines whether verifies that the server cert is known.
  static long sSSLVerifyHost;  // long: follow libcurl

  // default CURLOPT_SSL_VERIFYHOST is 2
  static const long DEFALT_VERIFYHOST_OPT = 2;  // long: follow libcurl

  // default CURLOPT_SSL_VERIFYPEER is 1
  static const long DEFALT_VERIFYPEER_OPT = 1;  // long: follow libcurl
};

}  // namespace http
}  // namespace aliyun

#endif  // ALIYUN_HTTP_HTTP_REQUEST_H_
