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

#include <gtest/gtest.h>

#include "aliyun/auth/hmac_sha1.h"
#include "aliyun/auth/hmac_sha256.h"
#include "aliyun/utils/string_utils.h"

using std::string;
using aliyun::auth::ISigner;
using aliyun::auth::HmacSha1;
using aliyun::auth::HmacSha256;
using aliyun::utils::StringUtils::hexDump;

TEST(HmacSha1, test) {
  HmacSha1 hs;

  string key = "AccessSecret";
  string msg = "this is a HmacSha1 test.";

  unsigned char hmac[HmacSha1::DIGEST_LENTH];
  hs.HMAC_SHA1(hmac, reinterpret_cast<const unsigned char *>(key.c_str()),
               key.length(),
               reinterpret_cast<const unsigned char *>(msg.c_str()),
               msg.length());

  EXPECT_EQ("47ff824f95bba3569f6bc8a91023b74b26230fad",
            hexDump(hmac, sizeof hmac, false));

  EXPECT_EQ("R/+CT5W7o1afa8ipECO3SyYjD60=", hs.signString(msg, key));

  EXPECT_EQ("axE3FUHgDyfm9/+Iep0HpZXrRwE=", HmacSha1::getInstance()->signString(
      "GET&%2F&AccessKeyId%3Dtestid%26Action%3DDescribeRegions%26"
          "Format%3DXML%26RegionId%3Dregion1%26SignatureMethod%3DHMAC-SHA1%26"
          "SignatureNonce%3DNwDAxvLU6tFE0DVb%26SignatureVersion%3D1.0%26"
          "TimeStamp%3D2012-12-26T10%253A33%253A56Z%26Version%3D2013-01-10",
      "testsecret&"));
}

TEST(HmacSha256, test) {
  HmacSha256 hs;

  string msg = "this is a ShaHmac256 test.";
  string key = "AccessSecret";

  unsigned char hmac[32];
  hs.HMAC_SHA256(hmac, reinterpret_cast<const unsigned char *>(key.c_str()),
                 key.length(),
                 reinterpret_cast<const unsigned char *>(msg.c_str()),
                 msg.length());

  EXPECT_EQ("3f4307077c25b5aff804e29ae71b7e3e28bd5a70cce92fa38026d0e36a742848",
            hexDump(hmac, sizeof hmac, false));

  EXPECT_EQ("P0MHB3wlta/4BOKa5xt+Pii9WnDM6S+jgCbQ42p0KEg=",
            hs.signString("this is a ShaHmac256 test.", "AccessSecret"));
}
