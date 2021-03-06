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
#include "aliyun/auth/credential.h"

using aliyun::auth::Credential;

TEST(CredentialTest, test) {
  Credential cred;
  std::cout << cred.getRefreshDate().toString() << std::endl;
  std::cout << cred.getExpiredDate().toString() << std::endl;

  Credential cred2("key2", "secret2");
  EXPECT_EQ("key2", cred2.getAccessKeyId());
  EXPECT_EQ("secret2", cred2.getAccessSecret());
  std::cout << "cred2:" << cred2.getRefreshDate().toString() << "\t";
  std::cout << cred2.getExpiredDate().toString() << std::endl;

  Credential cred3("key3", "secret3", 1);
  EXPECT_EQ("key3", cred3.getAccessKeyId());
  EXPECT_EQ("secret3", cred3.getAccessSecret());
  std::cout << "cred3:" << cred3.getRefreshDate().toString() << "\t";
  std::cout << cred3.getExpiredDate().toString() << std::endl;

  EXPECT_TRUE(cred.isExpired());
  EXPECT_TRUE(cred2.isExpired());
  EXPECT_FALSE(cred3.isExpired());
}
