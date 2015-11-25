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

#include <iostream>
#include <gtest/gtest.h>
#include "aliyun/opensearch/CloudsearchSearch.h"

using aliyun::opensearch::object::KeyTypeEnum;
using aliyun::opensearch::CloudsearchClient;
using aliyun::opensearch::CloudsearchSearch;

void testHttpStatus(std::string url, long status) {
  using aliyun::http::HttpRequest;
  using aliyun::http::HttpResponse;

  HttpRequest request(url);
  HttpResponse response = HttpResponse::getResponse(request);

  EXPECT_EQ(status, response.getStatus());
}

TEST(HTTP, status) {
  testHttpStatus("http://g.cn/generate_204", 204);
}

TEST(CloudsearchSearch, search) {
  std::map<std::string, std::string> opts;
  CloudsearchClient client("BBQtaPv7l14mNRDs",
                           "ebwFrJ0f72ttx53rNm6Tlviie0prGK",
                           "http://opensearch-cn-hangzhou.aliyuncs.com",
                           opts,
                           KeyTypeEnum::ALIYUN);

  CloudsearchSearch search(client);
  search.addIndex("sagent");

  std::string result = search.search();
  std::cout << result << std::endl;
  EXPECT_TRUE(result.length() != 0);
}