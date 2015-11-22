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

#include "HmacSha256.h"

#include <apr_errno.h>
#include <apr_pools.h>
#include <apr_random.h>

#include "../Exception.h"
#include "../utils/Base64Helper.h"

namespace aliyun {

namespace auth {

HmacSha256* HmacSha256::sInstance_ = NULL;

HmacSha256::HmacSha256()
    : pool_(NULL),
      hash_(NULL) {
  apr_status_t rc;
  char emsg[256];

  if ((rc = apr_pool_create(&pool_, NULL)) != APR_SUCCESS) {
    throw Exception(apr_strerror(rc, emsg, sizeof(emsg)));
  }
  hash_ = apr_crypto_sha256_new(pool_);
  if (NULL == hash_) {
    throw Exception(apr_strerror(rc, emsg, sizeof(emsg)));
  }
}

HmacSha256::~HmacSha256() {
  if (pool_ != NULL) {
    apr_pool_destroy(pool_);
  }
}

std::string HmacSha256::signString(std::string source, std::string accessSecret)
                                       throw (aliyun::Exception) {
  unsigned char hmac[DIGEST_LENTH];
  HMAC_SHA256(hmac,
              reinterpret_cast<const unsigned char*>(accessSecret.c_str()),
              accessSecret.length(),
              reinterpret_cast<const unsigned char*>(source.c_str()),
              source.length());
  return utils::Base64Helper::encode(hmac, DIGEST_LENTH);
}

void HmacSha256::HMAC_SHA256(unsigned char hmac[32], const unsigned char *key,
                             int key_len, const unsigned char *message,
                             int message_len) {
  unsigned char kopad[64], kipad[64];
  int i;
  unsigned char digest[32];

  if (key_len > 64) {
    key_len = 64;
  }

  for (i = 0; i < key_len; i++) {
    kopad[i] = key[i] ^ 0x5c;
    kipad[i] = key[i] ^ 0x36;
  }

  for (; i < 64; i++) {
    kopad[i] = 0 ^ 0x5c;
    kipad[i] = 0 ^ 0x36;
  }

  hash_->init(hash_);
  hash_->add(hash_, (const char *) kipad, 64);
  hash_->add(hash_, (const char *) message, (unsigned int) message_len);
  hash_->finish(hash_, digest);

  hash_->init(hash_);
  hash_->add(hash_, (const char *) kopad, 64);
  hash_->add(hash_, (const char *) digest, 32);
  hash_->finish(hash_, hmac);
}

HmacSha256* HmacSha256::getInstance() {
  if (sInstance_ == NULL) {
    static HmacSha256 stub;
    sInstance_ = &stub;
  }
  return sInstance_;
}
}  // namespace auth

}  // namespace aliyun

