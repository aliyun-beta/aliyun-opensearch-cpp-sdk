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

#ifndef CORE_AUTH_HMACSHA256_H_
#define CORE_AUTH_HMACSHA256_H_

#include <string>

#include "ISigner.h"

struct apr_crypto_hash_t;

struct apr_pool_t;

namespace aliyun {

namespace auth {

class HmacSha256 : public ISigner {
 public:
  const static int DIGEST_LENTH = 32;  // 256/8

  HmacSha256();

  ~HmacSha256();

  std::string getSignerName() {
    return "HMAC-SHA256";
  }

  std::string getSignerVersion() {
    return "1.0";
  }

  std::string signString(std::string source, std::string accessSecret) throw (aliyun::Exception);

  void HMAC_SHA256(unsigned char hmac[32], const unsigned char *key,
                   int key_len, const unsigned char *message, int message_len);

  static HmacSha256 * getInstance();

 private:
  static HmacSha256* sInstance_;

  apr_pool_t* pool_;
  apr_crypto_hash_t* hash_;
};

}  // namespace auth

}  // namespace aliyun

#endif  // CORE_AUTH_HMACSHA256_H_
