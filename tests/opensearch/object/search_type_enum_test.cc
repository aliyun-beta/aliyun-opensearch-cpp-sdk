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
#include "aliyun/opensearch/object/search_type_enum.h"

using aliyun::opensearch::object::SearchTypeEnum;

TEST(SearchTypeEnumTest, testAll) {
  SearchTypeEnum e1 = SearchTypeEnum::INVALID;
  EXPECT_EQ("INVALID", e1.toString());

  SearchTypeEnum e2(SearchTypeEnum::SCROLL);
  EXPECT_EQ("SCROLL", e2.toString());

  SearchTypeEnum e3("SEARCH");
  EXPECT_EQ("SEARCH", e3.toString());

  SearchTypeEnum e4 = SearchTypeEnum::SEARCH;
  EXPECT_TRUE(e4 == e3);
}
