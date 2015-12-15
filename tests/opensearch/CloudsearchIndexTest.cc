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
#include "aliyun/opensearch.h"

using std::string;
using aliyun::opensearch::object::KeyTypeEnum;
using aliyun::opensearch::CloudsearchIndex;
using aliyun::opensearch::CloudsearchClient;

class CloudsearchIndexTest: public ::testing::Test {
  CloudsearchClient *client_;

 public:
  virtual void SetUp() {
    std::map<string, string> opts;
    client_ = new CloudsearchClient(
        "BBQtaPv7l14mNRDs",
        "ebwFrJ0f72ttx53rNm6Tlviie0prGK",
        "http://opensearch-cn-hangzhou.aliyuncs.com",
        opts,
        KeyTypeEnum::ALIYUN);
  }

  CloudsearchClient &client() {
    return *client_;
  }

  virtual void TearDown() {
    delete client_;
  }
};

TEST_F(CloudsearchIndexTest, ctor) {
  CloudsearchIndex index("sagent", this->client());
  EXPECT_EQ("sagent", index.getIndexName());
}

TEST_F(CloudsearchIndexTest, status) {
  CloudsearchIndex index("sagent", this->client());
  string status = index.status();

  printf("status: %s\n", status.c_str());
  EXPECT_GT(status.length(), 0);
}
