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

#include "aliyun/utils/ParameterHelper.h"
#include "aliyun/auth/RoaSignatureComposer.h"

namespace aliyun {

namespace auth {

ISignatureComposer* RoaSignatureComposer::sInstance_ = NULL;

std::map<std::string, std::string> RoaSignatureComposer::refreshSignParameters(
    const std::map<string, string>& parameters, ISigner* signer,
    string accessKeyId, http::FormatType format) {
  std::map<string, string> result(parameters);
  result["Date"] = utils::ParameterHelper::getRFC2616Date(utils::Date());

  if (http::FormatType::INVALID == format)
    format = http::FormatType::RAW;
  result["Accept"] = http::FormatType::mapFormatToAccept(format);
  result["x-acs-signature-method"] = signer->getSignerName();
  result["x-acs-signature-version"] = signer->getSignerVersion();
  return result;
}

std::string RoaSignatureComposer::composeStringToSign(
    http::MethodType method, string uriPattern, ISigner* signer,
    const std::map<string, string>& queries,
    const std::map<string, string>& headers,
    const std::map<string, string>& paths) {
  string str = method.toString();
  str += HEADER_SEPARATOR;

  std::map<string, string>::const_iterator pos;

  pos = headers.find("Accept");
  if (pos != headers.end()) {
    str += pos->second;
  }
  str += HEADER_SEPARATOR;

  pos = headers.find("Content-MD5");
  if (pos != headers.end()) {
    str += pos->second;
  }
  str += HEADER_SEPARATOR;

  pos = headers.find("Content-Type");
  if (pos != headers.end()) {
    str += pos->second;
  }
  str += HEADER_SEPARATOR;

  pos = headers.find("Date");
  if (pos != headers.end()) {
    str += pos->second;
  }
  str += HEADER_SEPARATOR;

  string uri = replaceOccupiedParameters(uriPattern, paths);
  str += buildCanonicalHeaders(headers, "x-acs-");
  str += buildQueryString(uri, queries);
  return str;
}

ISignatureComposer* RoaSignatureComposer::getComposer() {
  if (NULL == sInstance_) {
    static RoaSignatureComposer stub;
    sInstance_ = &stub;
  }
  return sInstance_;
}

std::string RoaSignatureComposer::replaceOccupiedParameters(
    string url, const std::map<string, string>& paths) {
  string result = url;
  for (std::map<string, string>::const_iterator it = paths.begin();
      it != paths.end(); ++it) {
    string target = "[" + it->first + "]";
    string::size_type pos = result.find(target);
    if (pos != string::npos) {
      result.replace(pos, target.length(), it->second);
    }
  }
  return result;
}

std::string RoaSignatureComposer::buildCanonicalHeaders(
    const std::map<string, string>& headers, string headerBegin) {
  std::map<string, string> orderedMap;
  for (std::map<string, string>::const_iterator it = headers.begin();
      it != headers.end(); ++it) {
    string key = it->first;
    if (key.find(headerBegin) == 0) {  // begin with it.
      orderedMap[key] = it->second;
    }
  }
  string headerString;
  for (std::map<string, string>::iterator it = orderedMap.begin();
      it != orderedMap.end(); ++it) {
    headerString += it->first;
    headerString += ':';  // FIXME(xu): replace with ": " ?
    headerString += HEADER_SEPARATOR;
  }
  return headerString;
}

std::vector<std::string> RoaSignatureComposer::splitSubResource(string uri) {
  string::size_type idx = uri.find("?");
  std::vector<string> uriParts;
  if (string::npos != idx) {
    uriParts.push_back(uri.substr(0, idx));
    uriParts.push_back(uri.substr(idx + 1));
  } else {
    uriParts.push_back(uri);
  }
  return uriParts;
}

std::string RoaSignatureComposer::buildQueryString(
    string uri, std::map<string, string> queries) {
  std::vector<string> uriParts = splitSubResource(uri);
  std::map<string, string> sortMap;
  if (uriParts.size() > 1) {
    sortMap[uriParts[1]] = "";
  }

  string queryString = uriParts[0];
  if (sortMap.size() > 0) {
    queryString.append("?");  // parameter start symbol
  }

  for (std::map<string, string>::iterator it = sortMap.begin();
      it != sortMap.end(); ++it) {
    queryString += it->first;
    if (it->second.length() != 0) {
      queryString += ("=" + it->second);
    }
    queryString += QUERY_SEPARATOR;
  }
  string::size_type idx = queryString.find_last_of(QUERY_SEPARATOR);
  if (idx == queryString.length() - QUERY_SEPARATOR.length()) {
    queryString.resize(queryString.length() - QUERY_SEPARATOR.length());
  }
  return queryString;
}

}  // namespace auth

}  // namespace aliyun
