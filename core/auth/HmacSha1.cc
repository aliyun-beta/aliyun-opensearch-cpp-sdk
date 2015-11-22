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

#include "HmacSha1.h"

#include <apr_sha1.h>

#include "../Exception.h"
#include "../utils/Base64Helper.h"

namespace aliyun {

namespace auth {

HmacSha1* HmacSha1::sInstance_ = NULL;

std::string HmacSha1::signString(std::string source, std::string accessSecret)
                                     throw(aliyun::Exception) {
  unsigned char hmac[DIGEST_LENTH];
  HMAC_SHA1(hmac, reinterpret_cast<const unsigned char*>(accessSecret.c_str()),
            accessSecret.length(),
            reinterpret_cast<const unsigned char*>(source.c_str()),
            source.length());
  return utils::Base64Helper::encode(hmac, DIGEST_LENTH);
}

void HmacSha1::HMAC_SHA1(unsigned char hmac[20], const unsigned char *key,
                         int key_len, const unsigned char *message,
                         int message_len) {
  unsigned char kopad[64], kipad[64];
  int i;
  unsigned char digest[APR_SHA1_DIGESTSIZE];
  apr_sha1_ctx_t context;

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

  apr_sha1_init(&context);
  apr_sha1_update(&context, (const char *) kipad, 64);
  apr_sha1_update(&context, (const char *) message, (unsigned int) message_len);
  apr_sha1_final(digest, &context);

  apr_sha1_init(&context);
  apr_sha1_update(&context, (const char *) kopad, 64);
  apr_sha1_update(&context, (const char *) digest, 20);
  apr_sha1_final(hmac, &context);
}

HmacSha1* HmacSha1::getInstance() {
  if (sInstance_ == NULL) {
    static HmacSha1 stub;
    sInstance_ = &stub;
  }
  return sInstance_;
}
}  // namespace auth

}  // namespace aliyun
