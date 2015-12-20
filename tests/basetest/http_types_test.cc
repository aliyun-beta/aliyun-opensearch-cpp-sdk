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

#include "aliyun/exception.h"
#include "aliyun/http/format_type.h"
#include "aliyun/http/method_type.h"
#include "aliyun/http/protocol_type.h"

using aliyun::Exception;
using aliyun::http::FormatType;
using aliyun::http::MethodType;
using aliyun::http::ProtocolType;

TEST(HttpTypesTest, testFormatType) {
  enum FooEnum { UNKNOW };
  EXPECT_EQ(sizeof(FooEnum), sizeof(int));

  std::string testNames[] = {"application/xml", "application/json",
                             "application/octet-stream"};
  FormatType::Value testValues[] = {FormatType::XML, FormatType::JSON,
                                    FormatType::RAW};

  FormatType format;
  // construct from string, cover mapAcceptToFormat & mapFormatToAccept

  try {
    for (size_t i = 0; i < sizeof(testNames)/sizeof(testNames[0]); i++) {
      EXPECT_EQ(testNames[i],
                FormatType::mapFormatToAccept(FormatType(testValues[i])));
      EXPECT_EQ(FormatType(testValues[i]),
                FormatType::mapAcceptToFormat(testNames[i]));
    }
  } catch (Exception &e) {
    std::cout << "## Exception ##\n what: " << e.what() << std::endl;
    std::cout << "stack:\n" << e.stackTrace() << std::endl;
  }
}

TEST(HttpTypesTest, testProtocolType) {
  ProtocolType protocol;

  std::string testNames[] = { "http", "https", "unknow" };
  int testValues[] =
      {ProtocolType::HTTP, ProtocolType::HTTPS, ProtocolType::INVALID};

  for (size_t i = 0; i < sizeof(testNames) / sizeof(testNames[0]); i++) {
    try {
      protocol = ProtocolType(testNames[i]);
      EXPECT_EQ(testNames[i], protocol.toString());
      EXPECT_TRUE(testValues[i] == protocol);
    } catch (Exception& e) {
      std::cout << "## Exception ##\n what: " << e.what() << std::endl;
      std::cout << "stack:\n" << e.stackTrace() << std::endl;
    }
  }
}

TEST(HttpTypesTest, testMethodType) {
  EXPECT_TRUE(MethodType::INVALID == MethodType("INVALID"));
  EXPECT_TRUE(MethodType::GET == MethodType("GET"));
  EXPECT_TRUE(MethodType::GET == MethodType("get"));
  EXPECT_TRUE(MethodType::PUT == MethodType("PUT"));
  EXPECT_TRUE(MethodType::PUT == MethodType("put"));
  EXPECT_TRUE(MethodType::POST == MethodType("POST"));
  EXPECT_TRUE(MethodType::POST == MethodType("post"));
  EXPECT_TRUE(MethodType::HEAD == MethodType("HEAD"));
  EXPECT_TRUE(MethodType::HEAD == MethodType("head"));
  EXPECT_TRUE(MethodType::Delete == MethodType("DELETE"));
  EXPECT_TRUE(MethodType::Delete == MethodType("delete"));
  EXPECT_TRUE(MethodType::OPTIONS == MethodType("OPTIONS"));
  EXPECT_TRUE(MethodType::OPTIONS == MethodType("options"));
  EXPECT_EQ("INVALID", MethodType(MethodType::INVALID).toString());
  EXPECT_EQ("GET", MethodType(MethodType::GET).toString());
  EXPECT_EQ("PUT", MethodType(MethodType::PUT).toString());
  EXPECT_EQ("POST", MethodType(MethodType::POST).toString());
  EXPECT_EQ("HEAD", MethodType(MethodType::HEAD).toString());
  EXPECT_EQ("Delete", MethodType(MethodType::Delete).toString());
  EXPECT_EQ("OPTIONS", MethodType(MethodType::OPTIONS).toString());
}
