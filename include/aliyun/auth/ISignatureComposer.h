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

#ifndef CORE_AUTH_ISIGNATURECOMPOSER_H_
#define CORE_AUTH_ISIGNATURECOMPOSER_H_

#include <map>
#include <string>

#include "../http/FormatType.h"
#include "../http/MethodType.h"
#include "ISigner.h"

namespace aliyun {

namespace auth {

class ISignatureComposer {
 public:
  typedef std::string string;

  static const string QUERY_SEPARATOR;

  static const string HEADER_SEPARATOR;

  virtual ~ISignatureComposer() {
  }

  virtual std::map<string, string> refreshSignParameters(
      const std::map<string, string>& parameters, ISigner* signer,
      string accessKeyId, http::FormatType format) = 0;

  virtual string composeStringToSign(http::MethodType method, string uriPattern,
                                     ISigner* signer,
                                     const std::map<string, string>& queries,
                                     const std::map<string, string>& headers,
                                     const std::map<string, string>& paths) = 0;
};

}  // namespace auth

}  // namespace aliyun

#endif  // CORE_AUTH_ISIGNATURECOMPOSER_H_
