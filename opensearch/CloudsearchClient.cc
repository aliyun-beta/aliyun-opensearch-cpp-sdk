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

#include "CloudsearchClient.h"

#include "../core/Exception.h"
#include "../core/http/HttpRequest.h"

namespace aliyun {
namespace opensearch {

const std::string CloudsearchClient::DEFAULT_METHOD = "GET";
const std::string CloudsearchClient::METHOD_GET = "GET";
const std::string CloudsearchClient::METHOD_POST = "POST";

CloudsearchClient::CloudsearchClient(string accesskey, string secret,
                                     string host,
                                     const std::map<string, string>& opts,
                                     KeyTypeEnum keyType) {
  this->version_ = "v2";
  this->keyType_ = keyType;
  this->accesskey_ = accesskey;
  this->secret_ = secret;
  this->initialize("", "", host, opts);
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

} /* namespace opensearch */
} /* namespace aliyun */
