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
#include "aliyun/auth/OssSignatureComposer.h"

namespace aliyun {

namespace auth {

ISignatureComposer* OssSignatureComposer::sInstance_ = NULL;

std::map<std::string, std::string> OssSignatureComposer::refreshSignParameters(
    const std::map<string, string>& parameters, ISigner* signer,
    string accessKeyId, http::FormatType format) {
  std::map<string, string> params;
  params["Date"] = utils::ParameterHelper::getRFC2616Date(utils::Date());
  return params;
}

std::string OssSignatureComposer::composeStringToSign(
    http::MethodType method, string uriPattern, ISigner* signer,
    const std::map<string, string>& queries,
    const std::map<string, string>& headers,
    const std::map<string, string>& paths) {
  string str = method.toString();
  str += HEADER_SEPARATOR;

  std::map<string, string>::const_iterator pos;
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

  str += buildCanonicalHeaders(headers, "x-oss-");
  str += buildQueryString(uriPattern, queries);
  return str;
}

ISignatureComposer* OssSignatureComposer::getComposer() {
  if (sInstance_ == NULL) {
    static OssSignatureComposer stub;
    sInstance_ = &stub;
  }
  return sInstance_;
}

std::string OssSignatureComposer::buildQueryString(
    string uri, const std::map<string, string>& queries) {
  string queryString = uri;
  if (0 < queries.size())
    queryString.append("?");  // parameter start symbol
  for (std::map<string, string>::const_iterator it = queries.begin();
      it != queries.end(); ++it) {
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

