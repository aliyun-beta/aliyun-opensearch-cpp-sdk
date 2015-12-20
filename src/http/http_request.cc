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

#include <string.h>
#include <vector>

#include "aliyun/http/http_request.h"
#include "aliyun/utils/parameter_helper.h"
#include "aliyun/utils/string_utils.h"
#include "aliyun/utils/details/global_initializer.h"

namespace aliyun {
namespace http {

// long: follow libcurl API
long HttpRequest::sSSLVerifyHost = HttpRequest::DEFALT_VERIFYHOST_OPT;
long HttpRequest::sSSLVerifyPeer = HttpRequest::DEFALT_VERIFYPEER_OPT;


CurlException::CurlException(CURLcode rc)
    : Exception(curl_easy_strerror(rc)) {
}

CurlException::CurlException(std::string what)
    : Exception(what) {
}

CurlHandle::CurlHandle()
    : head_(0),
      post_(0),
      curl_(NULL) {
  curl_ = curl_easy_init();
  if (NULL == curl_) {
    throw CurlException("curl init fail");
  }
}

CurlHandle::~CurlHandle() {
  if (curl_) curl_easy_cleanup(curl_);
  if (head_) curl_slist_free_all(head_);
  if (post_) free(post_);  // free here
}

HttpRequest::HttpRequest() {
  method_ = MethodType::INVALID;
  contentType_ = FormatType::INVALID;
}

HttpRequest::HttpRequest(std::string url) {
  url_ = url;
  method_ = MethodType::INVALID;
  contentType_ = FormatType::INVALID;
}

HttpRequest::HttpRequest(std::string url,
                         const std::map<std::string, std::string>& headers) {
  url_ = url;
  headers_ = headers;
  method_ = MethodType::INVALID;
  contentType_ = FormatType::INVALID;
}

void HttpRequest::setContentType(FormatType contentType) {
  contentType_ = contentType;
  if (FormatType::INVALID != contentType_
      || FormatType::INVALID != contentType) {
    headers_["Content-Type"] = getContentTypeValue(contentType_, encoding_);
  } else {
    removeHeaderParameter("Content-Type");
  }
}

std::string HttpRequest::getHeaderValue(std::string name) const {
  std::string value;
  std::map<std::string, std::string>::const_iterator it;
  it = headers_.find(name);
  if (it != headers_.end()) {
    value = it->second;
  }
  return value;
}

void HttpRequest::putHeaderParameter(std::string name, std::string value) {
  if (name.length() != 0 && value.length() != 0) {
    headers_[name] = value;
  }
}

void HttpRequest::removeHeaderParameter(std::string name) {
  std::map<std::string, std::string>::iterator it = headers_.find(name);
  if (it != headers_.end()) {
    headers_.erase(it);
  }
}

void HttpRequest::setContent(std::string content, std::string encoding,
                             FormatType format) {
  if (content.length() == 0) {
    removeHeaderParameter("Content-MD5");
    removeHeaderParameter("Content-Length");
    removeHeaderParameter("Content-Type");
    contentType_ = FormatType::INVALID;
    content_.clear();
    encoding_.clear();
    return;
  }
  content_ = content;
  encoding_ = encoding;

  if (FormatType::INVALID != format) {
    contentType_ = format;
  } else {
    contentType_ = FormatType::RAW;
  }
  headers_["Content-MD5"] = aliyun::utils::ParameterHelper::md5Sum(content_);
  headers_["Content-Length"] = aliyun::utils::StringUtils::ToString(
      content_.length());
  headers_["Content-Type"] = getContentTypeValue(contentType_, encoding_);
}

void HttpRequest::prepareCurlHandle(CurlHandle* curl) {
  std::string url = url_;
  std::vector<std::string> urlArray;

  // split arguments for POST
  if (MethodType::POST == method_ && getContent().length() == 0) {
    std::string::size_type pos = 0;
    pos = url_.find("?");
    if (pos != std::string::npos) {
      urlArray.push_back(url_.substr(0, pos));
      urlArray.push_back(url_.substr(pos + 1, url_.length() - (pos + 1)));
      url = urlArray[0];
    }
  }
  if (url.length() == 0) {
    throw Exception("bad URL");
  }

#define curl_easy_setopt_throw(opt, val) do { \
      rc = curl_easy_setopt(*curl, opt, (val));      \
      if (rc != CURLE_OK) throw CurlException(rc);  \
  } while (0)

  CURLcode rc;
  curl_easy_setopt_throw(CURLOPT_URL, url.c_str());

  switch (method_) {
    case MethodType::GET:
      curl_easy_setopt_throw(CURLOPT_HTTPGET, 1);
      break;
    case MethodType::PUT:
      curl_easy_setopt_throw(CURLOPT_UPLOAD, 1);
      curl_easy_setopt_throw(CURLOPT_PUT, 1);
      break;
    case MethodType::HEAD:
      curl_easy_setopt_throw(CURLOPT_NOBODY, 1);
      break;
    case MethodType::POST:
      curl_easy_setopt_throw(CURLOPT_POST, 1);
      break;
    case MethodType::Delete:
      curl_easy_setopt_throw(CURLOPT_CUSTOMREQUEST, "DELETE");
      break;
    case MethodType::OPTIONS:
      curl_easy_setopt_throw(CURLOPT_CUSTOMREQUEST, "OPTIONS");
      break;
    default:
      break;
  }

  if (sSSLVerifyHost != DEFALT_VERIFYHOST_OPT) {
    curl_easy_setopt_throw(CURLOPT_SSL_VERIFYHOST, sSSLVerifyHost);
  }
  if (sSSLVerifyPeer != DEFALT_VERIFYPEER_OPT) {
    curl_easy_setopt_throw(CURLOPT_SSL_VERIFYPEER, sSSLVerifyPeer);
  }

  std::string contType = getHeaderValue("Content-Type");
  if (contType.length() == 0) {
    contType = getContentTypeValue(contentType_, encoding_);
    if (contType.length() != 0) {
      putHeaderParameter("Content-Type", contType);
    }
  }

  std::map<std::string, std::string>::iterator iter;
  for (iter = headers_.begin(); iter != headers_.end(); iter++) {
    std::string s = iter->first + ": " + iter->second;
    curl->head_ = curl_slist_append(curl->head_, s.c_str());
  }

  if (NULL != curl->head_) {
    curl_easy_setopt_throw(CURLOPT_HTTPHEADER, curl->head_);
  }

  if (MethodType::POST == method_ && urlArray.size() == 2) {
    curl->post_ = ::strdup(urlArray[1].c_str());  // duplicate in heap
    curl_easy_setopt_throw(CURLOPT_POSTFIELDS, curl->post_);
  }
}

std::string HttpRequest::getContentTypeValue(FormatType contentType,
                                             std::string encoding) {
  if (FormatType::INVALID != contentType && encoding.length() != 0) {
    return FormatType::mapFormatToAccept(contentType) + ";charset="
        + aliyun::utils::StringUtils::ToLowerCase(encoding);
  } else if (FormatType::INVALID != contentType) {
    return FormatType::mapFormatToAccept(contentType);
  }
  return "";  // empty string.
#undef curl_easy_setopt_throw
}

void HttpRequest::enableGzip(bool enable) {
  // TODO(xu): implement gzip on curl
}

}  // namespace http
}  // namespace aliyun
