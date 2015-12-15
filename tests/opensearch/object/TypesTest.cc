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
#include "aliyun/opensearch/object/KeyTypeEnum.h"
#include "aliyun/opensearch/object/SearchTypeEnum.h"
#include "aliyun/opensearch/object/SchemaTableFieldType.h"

using aliyun::opensearch::object::KeyTypeEnum;
using aliyun::opensearch::object::SearchTypeEnum;
using aliyun::opensearch::object::SchemaTableFieldType;

TEST(KeyTypeEnum, all) {
  KeyTypeEnum e;
  KeyTypeEnum e1 = KeyTypeEnum::ALIYUN;
  KeyTypeEnum e2 = KeyTypeEnum::OPENSEARCH;

  EXPECT_EQ("INVALID", e.toString());
  EXPECT_EQ("ALIYUN", e1.toString());
  EXPECT_EQ("OPENSEARCH", e2.toString());

  EXPECT_TRUE(e == KeyTypeEnum::INVALID);
  EXPECT_TRUE(e1 == KeyTypeEnum::ALIYUN);
  EXPECT_TRUE(e2 == KeyTypeEnum::OPENSEARCH);
}

TEST(SearchTypeEnum, all) {
  SearchTypeEnum e;
  SearchTypeEnum e1 = SearchTypeEnum::SCROLL;
  SearchTypeEnum e2 = SearchTypeEnum::SEARCH;

  EXPECT_EQ("INVALID", e.toString());
  EXPECT_EQ("SCROLL", e1.toString());
  EXPECT_EQ("SEARCH", e2.toString());

  EXPECT_TRUE(e == SearchTypeEnum::INVALID);
  EXPECT_TRUE(e1 == SearchTypeEnum::SCROLL);
  EXPECT_TRUE(e2 == SearchTypeEnum::SEARCH);
}

TEST(SchemaTableFieldType, basic) {
  // ctor
  SchemaTableFieldType e;
  SchemaTableFieldType e1 = SchemaTableFieldType::INT8;
  SchemaTableFieldType e2 = SchemaTableFieldType::UINT8;
  SchemaTableFieldType e3 = SchemaTableFieldType::INT16;
  SchemaTableFieldType e4 = SchemaTableFieldType::UINT16;
  SchemaTableFieldType e5 = SchemaTableFieldType::INT32;
  SchemaTableFieldType e6 = SchemaTableFieldType::UINT32;
  SchemaTableFieldType e7 = SchemaTableFieldType::INT64;
  SchemaTableFieldType e8 = SchemaTableFieldType::UINT64;
  SchemaTableFieldType e9 = SchemaTableFieldType::TEXT;
  SchemaTableFieldType e10 = SchemaTableFieldType::STRING;
  SchemaTableFieldType e11 = SchemaTableFieldType::FLOAT;
  SchemaTableFieldType e12 = SchemaTableFieldType::DOUBLE;

  // getTypeName
  EXPECT_EQ("INVALID", e.getTypeName());
  EXPECT_EQ("INT8", e1.getTypeName());
  EXPECT_EQ("UINT8", e2.getTypeName());
  EXPECT_EQ("INT16", e3.getTypeName());
  EXPECT_EQ("UINT16", e4.getTypeName());
  EXPECT_EQ("INT32", e5.getTypeName());
  EXPECT_EQ("UINT32", e6.getTypeName());
  EXPECT_EQ("INT64", e7.getTypeName());
  EXPECT_EQ("UINT64", e8.getTypeName());
  EXPECT_EQ("TEXT", e9.getTypeName());
  EXPECT_EQ("STRING", e10.getTypeName());
  EXPECT_EQ("FLOAT", e11.getTypeName());
  EXPECT_EQ("DOUBLE", e12.getTypeName());

  // oeprator==
  EXPECT_TRUE(e == SchemaTableFieldType::INVALID);
  EXPECT_TRUE(e1 == SchemaTableFieldType::INT8);
  EXPECT_TRUE(e2 == SchemaTableFieldType::UINT8);
  EXPECT_TRUE(e3 == SchemaTableFieldType::INT16);
  EXPECT_TRUE(e4 == SchemaTableFieldType::UINT16);
  EXPECT_TRUE(e5 == SchemaTableFieldType::INT32);
  EXPECT_TRUE(e6 == SchemaTableFieldType::UINT32);
  EXPECT_TRUE(e7 == SchemaTableFieldType::INT64);
  EXPECT_TRUE(e8 == SchemaTableFieldType::UINT64);
  EXPECT_TRUE(e9 == SchemaTableFieldType::TEXT);
  EXPECT_TRUE(e10 == SchemaTableFieldType::STRING);
  EXPECT_TRUE(e11 == SchemaTableFieldType::FLOAT);
  EXPECT_TRUE(e12 == SchemaTableFieldType::DOUBLE);
}

TEST(SchemaTableFieldType, advance) {
// INT8("INT8", "INT"), UINT8("UINT8", "INT"),
// INT16("INT16", "INT"), UINT16("UINT16", "INT"),
// INT32("INT32", "INT"), UINT32("INT32", "INT"),
// INT64("INT64", "INT"), UINT64("INT64", "INT"),
// TEXT("TEXT", "TEXT"), STRING("STRING", "TEXT"),
// FLOAT("FLOAT", "FLOAT"), DOUBLE("DOUBLE","FLOAT");
  SchemaTableFieldType e;
  SchemaTableFieldType e1("INT8", "INT");
  SchemaTableFieldType e2("UINT8", "INT");
  SchemaTableFieldType e3("INT16", "INT");
  SchemaTableFieldType e4("UINT16", "INT");
  SchemaTableFieldType e5("INT32", "INT");
  SchemaTableFieldType e6("UINT32", "INT");
  SchemaTableFieldType e7("INT64", "INT");
  SchemaTableFieldType e8("UINT64", "INT");
  SchemaTableFieldType e9("TEXT", "TEXT");
  SchemaTableFieldType e10("STRING", "TEXT");
  SchemaTableFieldType e11("FLOAT", "FLOAT");
  SchemaTableFieldType e12("DOUBLE", "FLOAT");

  // getTypeName
  EXPECT_EQ("INVALID", e.getTypeName());
  EXPECT_EQ("INT8", e1.getTypeName());
  EXPECT_EQ("UINT8", e2.getTypeName());
  EXPECT_EQ("INT16", e3.getTypeName());
  EXPECT_EQ("UINT16", e4.getTypeName());
  EXPECT_EQ("INT32", e5.getTypeName());
  EXPECT_EQ("UINT32", e6.getTypeName());
  EXPECT_EQ("INT64", e7.getTypeName());
  EXPECT_EQ("UINT64", e8.getTypeName());
  EXPECT_EQ("TEXT", e9.getTypeName());
  EXPECT_EQ("STRING", e10.getTypeName());
  EXPECT_EQ("FLOAT", e11.getTypeName());
  EXPECT_EQ("DOUBLE", e12.getTypeName());

  // oeprator==
  EXPECT_TRUE(e == SchemaTableFieldType::INVALID);
  EXPECT_TRUE(e1 == SchemaTableFieldType::INT8);
  EXPECT_TRUE(e2 == SchemaTableFieldType::UINT8);
  EXPECT_TRUE(e3 == SchemaTableFieldType::INT16);
  EXPECT_TRUE(e4 == SchemaTableFieldType::UINT16);
  EXPECT_TRUE(e5 == SchemaTableFieldType::INT32);
  EXPECT_TRUE(e6 == SchemaTableFieldType::UINT32);
  EXPECT_TRUE(e7 == SchemaTableFieldType::INT64);
  EXPECT_TRUE(e8 == SchemaTableFieldType::UINT64);
  EXPECT_TRUE(e9 == SchemaTableFieldType::TEXT);
  EXPECT_TRUE(e10 == SchemaTableFieldType::STRING);
  EXPECT_TRUE(e11 == SchemaTableFieldType::FLOAT);
  EXPECT_TRUE(e12 == SchemaTableFieldType::DOUBLE);
}
