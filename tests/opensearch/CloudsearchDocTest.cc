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
#include "aliyun/utils/Date.h"
#include "aliyun/reader/JsonReader.h"
#include "aliyun/opensearch/CloudsearchDoc.h"
#include "aliyun/opensearch/CloudsearchClient.h"

using std::string;
using aliyun::utils::Date;
using aliyun::reader::JsonReader;
using aliyun::opensearch::object::KeyTypeEnum;
using aliyun::opensearch::CloudsearchDoc;
using aliyun::opensearch::CloudsearchClient;

TEST(CloudsearchDoc, ctor) {
  std::map<string, string> opts;
  CloudsearchClient client("key", "secret", "host", opts, KeyTypeEnum::ALIYUN);

  CloudsearchDoc doc("index", client);
  std::map<string, string> fields;
  fields["keyword"] = "SSD";

  doc.add(fields);

  doc.getDebugInfo();
}


TEST(CloudsearchDoc, add) {
  std::map<string, string> opts;
  CloudsearchClient client("BBQtaPv7l14mNRDs",
                           "ebwFrJ0f72ttx53rNm6Tlviie0prGK",
                           "http://opensearch-cn-hangzhou.aliyuncs.com",
                           opts,
                           KeyTypeEnum::ALIYUN);

  CloudsearchDoc doc("sagent", client);

  std::map<string, string> fields;
  fields["id"] = "5";
  fields["title"] = "this is the title";
  fields["body"] = "this is the body! modified on " + Date::currentLocalDate().toString();
  doc.add(fields);

  string response = doc.push("main");

  JsonReader reader;
  std::map<string, string> result = reader.read(response, "response");

  EXPECT_TRUE(result.find("response.status") != result.end());
  EXPECT_TRUE(result["response.status"] == "OK");
}

TEST(CloudsearchDoc, remove) {
  std::map<string, string> opts;
  CloudsearchClient client("BBQtaPv7l14mNRDs",
                           "ebwFrJ0f72ttx53rNm6Tlviie0prGK",
                           "http://opensearch-cn-hangzhou.aliyuncs.com",
                           opts,
                           KeyTypeEnum::ALIYUN);

  CloudsearchDoc doc("sagent", client);

  std::map<string, string> fields;
  fields["id"] = "5";

  doc.remove(fields);
  string response = doc.push("main");

  JsonReader reader;
  std::map<string, string> result = reader.read(response, "response");

  EXPECT_TRUE(result.find("response.status") != result.end());
  EXPECT_TRUE(result["response.status"] == "OK");
}

TEST(CloudsearchDoc, update) {
  std::map<string, string> opts;
  CloudsearchClient client("BBQtaPv7l14mNRDs",
                           "ebwFrJ0f72ttx53rNm6Tlviie0prGK",
                           "http://opensearch-cn-hangzhou.aliyuncs.com",
                           opts,
                           KeyTypeEnum::ALIYUN);

  CloudsearchDoc doc("sagent", client);

  std::map<string, string> fields;
  fields["id"] = "5";
  fields["title"] = "this is the title";
  fields["body"] = "this is the body! modified on " + Date::currentLocalDate().toString();
  doc.add(fields);

  string response = doc.push("main");

  JsonReader reader;
  std::map<string, string> result = reader.read(response, "response");

  EXPECT_TRUE(result.find("response.status") != result.end());
  EXPECT_TRUE(result["response.status"] == "OK");

  fields["body"] = "this is the body! modified on " + Date::currentLocalDate().toString();
  doc.update(fields);

  response = doc.push("main");

  result = reader.read(response, "response");

  EXPECT_TRUE(result.find("response.status") != result.end());
  EXPECT_TRUE(result["response.status"] == "OK");
}
