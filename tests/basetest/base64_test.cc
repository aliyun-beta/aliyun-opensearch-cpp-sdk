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
#include "aliyun/utils/base64_helper.h"

using std::string;
using aliyun::utils::Base64Helper;

TEST(Base64, encode) {
  string encoding = "UTF-8";
  string encode1 = Base64Helper::encode("1234567890", encoding);
  string encode2 = Base64Helper::encode("abcdefghijklmnopqrstuvwxyz", encoding);
  EXPECT_EQ("MTIzNDU2Nzg5MA==", encode1);
  EXPECT_EQ("YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXo=", encode2);

  string decode1 = Base64Helper::decode(encode1, encoding);
  string decode2 = Base64Helper::decode(encode2, encoding);
  EXPECT_EQ("1234567890", decode1);
  EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", decode2);
}
