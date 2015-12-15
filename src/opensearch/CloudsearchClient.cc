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

#include "aliyun/opensearch/CloudsearchClient.h"

namespace aliyun {
namespace opensearch {

using std::string;

const std::string CloudsearchClient::DEFAULT_METHOD = "GET";
const std::string CloudsearchClient::METHOD_GET = "GET";
const std::string CloudsearchClient::METHOD_POST = "POST";

CloudsearchClient::CloudsearchClient(string accesskey, string secret,
                                     string host,
                                     const std::map<string, string>& opts,
                                     KeyTypeEnum keyType) {
  this->initialize("", "", host, opts);
  this->version_ = "v2";
  this->keyType_ = keyType;
  this->accesskey_ = accesskey;
  this->secret_ = secret;
}

CloudsearchClient::CloudsearchClient(string clientId, string clientSecret,
                                     string host,
                                     const std::map<string, string>& opts) {
  this->initialize(clientId, clientSecret, host, opts);
}

void CloudsearchClient::initialize(const string& clientId,
                                   const string& clientSecret,
                                   const string& host,
                                   const std::map<string, string>& opts) {
  version_ = "v2";
  keyType_ = KeyTypeEnum::OPENSEARCH;
  clientId_ = clientId;
  clientSecret_ = clientSecret;
  host_ = host;

  if (host.length() == 0) {
    throw aliyun::Exception("UnknownHostException");
  }

  if (opts.size() > 0) {
    if (opts.find("version") != opts.end()) {
      version_ = opts.find("version")->second;
    }

    if (opts.find("gzip") != opts.end()) {
      http::HttpRequest::enableGzip(true);
    }

    if (opts.find("timeout") != opts.end()) {
      // TODO(xu): set http timeout
    }

    if (opts.find("connect_timeout") != opts.end()) {
      // TODO(xu): set http connect timeout
    }
  }

  baseURI_ =
      (host_.find_last_of("/") == host_.length() - 1) ?
          host_.substr(0, host_.length() - 1) : host_;
}

void CloudsearchClient::setMaxConnections(int maxConns) {
  if (maxConns > 0) {
    // TODO(xu): set maximum connnections
  }
}

string CloudsearchClient::call(string path,
                               const std::map<string, string>& params,
                               string method, bool isPB, string& debugInfo) {
  string uri;
  if (this->keyType_ == KeyTypeEnum::OPENSEARCH) {
    uri = '/' + this->version_ + "/api";
  }
  string url = this->baseURI_ + uri + path;

  std::map<string, string> parameters(params);
  if (this->keyType_ == KeyTypeEnum::OPENSEARCH) {
    parameters["client_id"] = this->clientId_;
    parameters["nonce"] = getNonce();
    parameters["sign"] = doSign(&parameters);
  } else if (this->keyType_ == KeyTypeEnum::ALIYUN) {
    parameters["Version"] = "v2";
    parameters["AccessKeyId"] = this->accesskey_;
    parameters["Timestamp"] = utils::ParameterHelper::getISO8601Date(
        utils::Date());
    parameters["SignatureMethod"] = "HMAC-SHA1";
    parameters["SignatureVersion"] = "1.0";
    parameters["SignatureNonce"] = utils::ParameterHelper::getUUID();
    parameters["Signature"] = getAliyunSign(&parameters, method);
  }
  if (method.length() == 0) {
    method = DEFAULT_METHOD;
  }

  debugInfo.resize(0);
  debugInfo.append(url + buildHttpParameterString(parameters));

  return this->doRequest(url, parameters, method, isPB);
}

string CloudsearchClient::getNonce() {
  time_t timestamp = ::time(NULL);
  string timeStr = utils::StringUtils::ToString(timestamp);
  string signStr = this->clientId_ + this->clientSecret_ + timeStr;
  string encoded = utils::StringUtils::ToEncoding(signStr, "UTF-8");
  return utils::ParameterHelper::md5hex(encoded) + "." + timeStr;
}

string CloudsearchClient::buildQuery(const std::map<string, string>& params) {
  string query;
  for (std::map<string, string>::const_iterator it = params.begin();
       it != params.end(); it++) {
    query += '&' + auth::AcsURLEncoder::encode(it->first);
    query += '=' + auth::AcsURLEncoder::encode(it->second);
  }
  return query.substr(1);
}

string CloudsearchClient::doSign(std::map<string, string>* params) {
  bool hasSignMode = false;
  string itemsValue;
  std::map<string, string>::iterator sign = params->find("sign_mode");
  std::map<string, string>::iterator items = params->find("items");
  if (sign != params->end() && sign->second == "1" && items != params->end()) {
    hasSignMode = true;
    itemsValue = items->second;
    params->erase(items);
  }

  string query = buildQuery(*params) + this->clientSecret_;
  string enc = utils::StringUtils::ToEncoding(query, "UTF-8");
  string md5 = utils::ParameterHelper::md5hex(enc);
  if (hasSignMode) {
    (*params)["items"] = itemsValue;
  }
  return md5;
}

string CloudsearchClient::buildHttpParameterString(
    const std::map<string, string>& params) {
  if (params.size() == 0) {
    return "";
  }
  string str;
  for (std::map<string, string>::const_iterator it = params.begin();
       it != params.end(); ++it) {
    string key = auth::AcsURLEncoder::percentEncode(it->first);
    string val = auth::AcsURLEncoder::percentEncode(it->second);
    str += "&" + key + "=" + val;
  }
  return "?" + str.substr(1);
}

string CloudsearchClient::getAliyunSign(std::map<string, string>* params,
                                        string method) {
  bool hasSignMode = false;
  string itemsValue;
  std::map<string, string>::iterator sign = params->find("sign_mode");
  std::map<string, string>::iterator items = params->find("items");
  if (sign != params->end() && sign->second == "1" && items != params->end()) {
    hasSignMode = true;
    itemsValue = items->second;
    params->erase(items);
  }

  string strToSign = buildQuery(*params);
  strToSign = method + "&%2F&"
      + auth::AcsURLEncoder::percentEncode(strToSign);
  string signature = auth::HmacSha1::getInstance()->signString(
      strToSign, this->secret_ + "&");

  if (hasSignMode) {
    (*params)["items"] = itemsValue;
  }
  return signature;
}

string CloudsearchClient::doRequest(string url,
                                    const std::map<string, string>& params,
                                    string method, bool isPB) {
  url = url + buildHttpParameterString(params);
  http::HttpRequest request(url);
  request.setMethod(method);

  http::HttpResponse response = http::HttpResponse::getResponse(request);

  string result = response.getContent();
  if (isPB) {
    // TODO(xu): handle response content encoding.
  }
  return result;
}

}  // namespace opensearch
}  // namespace aliyun
