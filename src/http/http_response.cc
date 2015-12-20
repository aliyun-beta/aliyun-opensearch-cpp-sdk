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

#include <stddef.h>
#include <string.h>

#include "aliyun/http/method_type.h"
#include "aliyun/http/http_response.h"
#include "aliyun/utils/string_utils.h"

namespace aliyun {
namespace http {

std::string HttpResponse::getHeaderValue(std::string name) const {
  std::map<std::string, std::string>::const_iterator it;
  it = headers_.find(name);
  if (it != headers_.end())
    return it->second;
  it = headers_.find(utils::StringUtils::ToLowerCase(name));
  if (it != headers_.end())
    return it->second;
  return "";
}

struct HttpTransaction {
  CURL* curl_;
  HttpRequest* request_;
  HttpResponse* response_;
  size_t bodySends_;
  size_t bodyReceives_;

  enum {
    INIT,
    HEADER,
    BODY_IN,
    BODY_OUT,
    ABORT,
    DONE,
  } state_;

  HttpTransaction(CURL* curl, HttpRequest* req, HttpResponse* resp)
      : curl_(curl),
        request_(req),
        response_(resp),
        bodySends_(0),
        bodyReceives_(0),
        state_(INIT) {
  }
};

static void updateTransactionInfo(HttpTransaction* t) {
#define curl_easy_getinfo_throw(curl, info, ptr) \
  rc = curl_easy_getinfo(curl, info, ptr);       \
  if (rc != CURLE_OK) throw CurlException(rc);

  // DONE: get response code, etc.
  CURLcode rc;
  long status = 0;  // follow libcurl API
  curl_easy_getinfo_throw(t->curl_, CURLINFO_RESPONSE_CODE, &status);
  t->response_->setStatus(status);
}

static size_t ResponseHeaderHandler(char *ptr, size_t size, size_t nmemb,
                                    void *userdata) {
  HttpTransaction* t = reinterpret_cast<HttpTransaction*>(userdata);
  size_t length = size * nmemb;

  t->state_ = HttpTransaction::HEADER;
  char* colon = ::strstr(ptr, ": ");
  if (colon != NULL) {
    std::string name(ptr, colon);
    char* crlf = ::strstr(ptr, "\r\n");
    std::string value(colon + 2, crlf ? crlf : ptr + length);
    t->response_->putHeaderParameter(name, value);
  }
  return length;
}

static size_t ResponseBodyHandler(char *ptr, size_t size, size_t nmemb,
                                  void *userdata) {
  HttpTransaction* t = reinterpret_cast<HttpTransaction*>(userdata);
  size_t length = size * nmemb;

  // DONE: handle http bodys.
  t->state_ = HttpTransaction::BODY_IN;
  t->response_->content().append(ptr, ptr + length);
  t->bodyReceives_ += length;
  return length;
}

static size_t RequestBodyHandler(char *ptr, size_t size, size_t nmemb,
                                 void *userdata) {
  HttpTransaction* t = reinterpret_cast<HttpTransaction*>(userdata);
  size_t buffLen = size * nmemb;  // internal body buffer length.
  size_t contLen = t->request_->getContent().length();
  size_t bodyLeft = contLen - t->bodySends_;

  t->state_ = HttpTransaction::BODY_OUT;
  if (bodyLeft > 0) {
    size_t sendLen = bodyLeft < buffLen ? bodyLeft : buffLen;
    char* sendPtr = &(t->request_->getContent()[t->bodySends_]);
    ::memcpy(ptr, sendPtr, sendLen);
    t->bodySends_ += sendLen;
    return sendLen;
  }
  return 0;
}

HttpResponse HttpResponse::getResponse(HttpRequest request) {
  CURLcode rc;
  HttpResponse response;

  CurlHandle curl;
  request.prepareCurlHandle(&curl);
  HttpTransaction trans(curl, &request, &response);

#define curl_easy_setopt_throw(opt, val)       \
  rc = curl_easy_setopt(curl, opt, (val));     \
  if (rc != CURLE_OK) throw CurlException(rc);

  // set up callbacks
  if (request.getMethod() == MethodType::PUT
      || request.getMethod() == MethodType::POST) {
    curl_easy_setopt_throw(CURLOPT_READDATA, &trans);
    curl_easy_setopt_throw(CURLOPT_READFUNCTION, RequestBodyHandler);
    curl_easy_setopt_throw(CURLOPT_INFILESIZE_LARGE,
                           request.getContent().size());
  }

  curl_easy_setopt_throw(CURLOPT_HEADERDATA, &trans);
  curl_easy_setopt_throw(CURLOPT_HEADERFUNCTION, ResponseHeaderHandler);

  curl_easy_setopt_throw(CURLOPT_WRITEDATA, &trans);
  curl_easy_setopt_throw(CURLOPT_WRITEFUNCTION, ResponseBodyHandler);

  curl_easy_setopt_throw(CURLOPT_FILETIME, 1);
  curl_easy_setopt_throw(CURLOPT_NOSIGNAL, 1);
  curl_easy_setopt_throw(CURLOPT_TCP_NODELAY, 1);  // disable Nagle
  curl_easy_setopt_throw(CURLOPT_NETRC, CURL_NETRC_IGNORED);

#ifdef ALIYUN_TRACE
  curl_easy_setopt_throw(curl, CURLOPT_VERBOSE, 1);
#endif
  rc = curl_easy_perform(curl);
  if (rc != 0)
    throw CurlException(rc);
  updateTransactionInfo(&trans);

  parseParameters(&response);
  return response;
}

void HttpResponse::parseParameters(HttpResponse* response) {
  using aliyun::utils::StringUtils::ToUpperCase;
  string type = response->getHeaderValue("Content-Type");
  if (type.length() > 0) {
    response->setEncoding("UTF-8");

    // application/xml
    // text/xml;charset=UTF-8
    string::size_type pos = type.find(';');
    FormatType format = FormatType::mapAcceptToFormat(type.substr(0, pos));
    response->setContentType(format);
    if (type.find('=') != string::npos) {
      pos = type.find('=');
      if (pos != string::npos) {
        string enc = type.substr(pos + 1);
        response->setEncoding(ToUpperCase(enc));
      }
    }
  }
}

}  // namespace  http
}  // namespace aliyun

