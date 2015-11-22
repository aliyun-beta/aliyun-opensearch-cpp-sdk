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

#ifndef CORE_AUTH_ROASIGNATURECOMPOSER_H_
#define CORE_AUTH_ROASIGNATURECOMPOSER_H_

#include <map>
#include <string>
#include <vector>

#include "../http/FormatType.h"
#include "../http/MethodType.h"
#include "ISignatureComposer.h"

namespace aliyun {

namespace auth {

class RoaSignatureComposer : public ISignatureComposer {
 public:
  typedef std::string string;

  std::map<string, string> refreshSignParameters(
      const std::map<string, string>& parameters, ISigner* signer,
      string accessKeyId, http::FormatType format);

  string composeStringToSign(http::MethodType method, string uriPattern,
                             ISigner* signer,
                             const std::map<string, string>& queries,
                             const std::map<string, string>& headers,
                             const std::map<string, string>& paths);

  static ISignatureComposer * getComposer();

  static string replaceOccupiedParameters(
      string url, const std::map<string, string>& paths);

 protected:

  string buildCanonicalHeaders(const std::map<string, string>& headers,
                               string headerBegin);

  std::vector<string> splitSubResource(string uri);

 private:
  string buildQueryString(string uri, std::map<string, string> queries);

 private:
  static ISignatureComposer* sInstance_;
};

}  // namespace auth

}  // namespace aliyun

#endif  // CORE_AUTH_ROASIGNATURECOMPOSER_H_
