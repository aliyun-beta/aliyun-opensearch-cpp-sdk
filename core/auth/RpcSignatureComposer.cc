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

#include "RpcSignatureComposer.h"

#include <string>

#include "../utils/Date.h"
#include "../utils/ParameterHelper.h"
#include "AcsURLEncoder.h"
#include "ISigner.h"

namespace aliyun {

namespace auth {

ISignatureComposer* RpcSignatureComposer::sInstance_ = NULL;

std::map<std::string, std::string> RpcSignatureComposer::refreshSignParameters(
    const std::map<string, string>& parameters, ISigner* signer,
    string accessKeyId, http::FormatType format) {
  std::map<string, string> params(parameters);
  params["Timestamp"] = utils::ParameterHelper::getISO8601Date(utils::Date());
  params["SignatureMethod"] = signer->getSignerName();
  params["SignatureVersion"] = signer->getSignerVersion();
  params["SignatureNonce"] = utils::ParameterHelper::getUUID();
  params["AccessKeyId"] = accessKeyId;

  if (format != http::FormatType::INVALID) {
    params["Format"] = format.toString();
  }
  return params;
}

std::string RpcSignatureComposer::composeStringToSign(
    http::MethodType method, string uriPattern, ISigner* signer,
    const std::map<string, string>& queries,
    const std::map<string, string>& headers,
    const std::map<string, string>& paths) {
  string cat;
  for (std::map<string, string>::const_iterator it = queries.begin();
      it != queries.end(); ++it) {
    cat += QUERY_SEPARATOR;
    cat += AcsURLEncoder::percentEncode(it->first);
    cat += "=";
    cat += AcsURLEncoder::percentEncode(it->second);
  }

  string str = method.toString();
  str += QUERY_SEPARATOR;
  str += AcsURLEncoder::percentEncode("/");
  str += QUERY_SEPARATOR;
  str += AcsURLEncoder::percentEncode(cat.substr(1));
  return str;
}

ISignatureComposer* RpcSignatureComposer::getComposer() {
  if (sInstance_ == NULL) {
    static RpcSignatureComposer stub;
    sInstance_ = &stub;
  }
  return sInstance_;
}

}  // namespace auth

}  // namespace aliyun

