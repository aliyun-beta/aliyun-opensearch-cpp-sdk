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

using aliyun::opensearch::object::KeyTypeEnum;
using aliyun::opensearch::CloudsearchClient;
using aliyun::opensearch::CloudsearchSearch;

class CloudsearchSearchTest : public ::testing::Test {
  std::map<std::string, std::string> opts_;
  CloudsearchClient client_;
 protected:
  CloudsearchSearch search_;
 public:
  CloudsearchSearchTest() :
      opts_(),
      client_("BBQtaPv7l14mNRDs",
              "ebwFrJ0f72ttx53rNm6Tlviie0prGK",
              "http://opensearch-cn-hangzhou.aliyuncs.com",
              opts_,
              KeyTypeEnum::ALIYUN),
      search_(client_) {
  }
};

TEST_F(CloudsearchSearchTest, testSearch) {
  search_.clear();
  search_.addIndex("sagent");
  std::string result = search_.search();
  std::cout << "result lenth:" << result.length() << std::endl;
  EXPECT_GT(result.length(), 0);
}

TEST_F(CloudsearchSearchTest, testGettersAndSetters) {
  search_.clear();

  std::vector<std::string> idx;
  search_.setIndex(idx);
  EXPECT_EQ(idx.size(), search_.getSearchIndexes().size());

  std::string formula = "formula";
  search_.setFormulaName(formula);
  EXPECT_EQ(formula, search_.getFormulaName());

  std::string format = "format";
  search_.setFormat(format);
  EXPECT_EQ(format, search_.getFormat());

  int hit = 0x21349823;
  search_.setHits(hit);
  EXPECT_EQ(hit, search_.getHits());

  int startHit = 0x12341234;
  search_.setStartHit(startHit);
  EXPECT_EQ(startHit, search_.getStartHit());

  std::string query = "query";
  search_.setQueryString(query);
  EXPECT_EQ(query, search_.getQuery());

  std::string pair = "pair";
  search_.setPair(pair);
  EXPECT_EQ(pair, search_.getPair());

  std::string scroll_expire = "1234";
  search_.setScrollExpire(scroll_expire);
  EXPECT_EQ(scroll_expire, search_.getScrollExpire());

  int expire = 1234;
  std::string last_expire = search_.getScrollExpire();
  search_.setScrollExpire(expire);
  EXPECT_EQ(last_expire, search_.getScrollExpire());

  std::string search_type = "search_type";
  search_.setSearchType(search_type);
  EXPECT_EQ(search_type, search_.getSearchType());

  std::string scroll_id = "scroll_id";
  search_.setScrollId(scroll_id);
  EXPECT_EQ(scroll_id, search_.getScrollId());

  search_.addFilter("filter1");
  EXPECT_EQ("filter1", search_.getFilter());
  search_.addFilter("filter2", "OR");
  EXPECT_EQ("filter1 OR filter2", search_.getFilter());
  search_.addFilter("filter3", "AND");
  EXPECT_EQ("filter1 OR filter2 AND filter3", search_.getFilter());

  search_.addFetchField("field1");
  std::vector<std::string> fields;
  fields.push_back("field2");
  fields.push_back("field3");
  search_.addFetchFields(fields);
  EXPECT_EQ(3, search_.getFetchFields().size());

  search_.addQpName("qpname1");
  std::vector<std::string> qpnames;
  qpnames.push_back("qpname2");
  qpnames.push_back("qpname3");
  search_.addQpNames(qpnames);
  EXPECT_EQ(3, search_.getQpNames().size());

  search_.addCustomConfig("cc-key1", "cc-value1");
  search_.addCustomConfig("cc-key2", "cc-value2");
  search_.removeCustomConfig("cc-key1");

  search_.addCustomParam("param-key", "param-value");
  EXPECT_EQ("param-value", search_.getCustomParam().find("param-key")->second);
}
