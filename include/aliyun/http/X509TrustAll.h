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

#ifndef ALIYUN_HTTP_X509TRUSTALL_H_
#define ALIYUN_HTTP_X509TRUSTALL_H_

#include "HttpRequest.h"

namespace aliyun {
namespace http {

// scope based HTTPS trust all.
// using RAII store/resume HTTPS verify flag.
// FIXME(xu): ScopedTrust maybe better ?
class X509TrustAll {
  int hostOptionCache_;
  int peerOptionCache_;

 public:
  X509TrustAll() {
    hostOptionCache_ = HttpRequest::getSslVerifyHost();
    peerOptionCache_ = HttpRequest::getSslVerifyPeer();
    HttpRequest::setSSLVerifyHost(0);  // trust any host
    HttpRequest::setSSLVerifyPeer(0);  // trust any peer
  }

  ~X509TrustAll() {
    HttpRequest::setSSLVerifyHost(hostOptionCache_);
    HttpRequest::setSSLVerifyPeer(peerOptionCache_);
  }
};

}  // namespace http
}  // namespace aliyun

#endif  // ALIYUN_HTTP_X509TRUSTALL_H_
