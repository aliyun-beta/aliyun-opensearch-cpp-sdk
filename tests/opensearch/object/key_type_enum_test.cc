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
#include "aliyun/opensearch/object/key_type_enum.h"

using aliyun::opensearch::object::KeyTypeEnum;

TEST(KeyTypeEnumTest, testCtor) {
  KeyTypeEnum e1 = KeyTypeEnum::INVALID;
  EXPECT_EQ("INVALID", e1.toString());

  KeyTypeEnum e2 = KeyTypeEnum::ALIYUN;
  EXPECT_EQ("ALIYUN", e2.toString());

  KeyTypeEnum e3 = KeyTypeEnum::OPENSEARCH;
  EXPECT_EQ("OPENSEARCH", e3.toString());

  KeyTypeEnum e4("INVALID");

  EXPECT_TRUE(KeyTypeEnum(KeyTypeEnum::INVALID) == KeyTypeEnum("INVALID"));
  EXPECT_TRUE(KeyTypeEnum(KeyTypeEnum::ALIYUN) == KeyTypeEnum("ALIYUN"));
  EXPECT_TRUE(KeyTypeEnum(KeyTypeEnum::OPENSEARCH) == KeyTypeEnum("OPENSEARCH"));
}
