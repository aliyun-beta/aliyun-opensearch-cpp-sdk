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
#include "aliyun/opensearch/object/schema_table_field.h"

using aliyun::opensearch::object::SchemaTableField;
using aliyun::opensearch::object::SchemaTableFieldType;

TEST(SchemaTableFieldTest, testCtor) {
  SchemaTableField schemaTableField;

  /*
  this->display_ = true;
  this->primarykey_ = false;
  this->multi_ = false;
  this->filter_ = false;
  this->search_ = false;
  this->aggregate_ = false;
  */
  EXPECT_TRUE(true == schemaTableField.isDisplay());
  EXPECT_TRUE(false == schemaTableField.isPrimarykey());
  EXPECT_TRUE(false == schemaTableField.isMulti());
  EXPECT_TRUE(false == schemaTableField.isFilter());
  EXPECT_TRUE(false == schemaTableField.isSearch());
  EXPECT_TRUE(false == schemaTableField.isAggregate());
}

TEST(SchemaTableFieldTest, testGettersAndSetters) {
  SchemaTableField schemaTableField;

  schemaTableField.setDisplay(false);
  EXPECT_FALSE(schemaTableField.isDisplay());

  schemaTableField.setPrimarykey(true);
  EXPECT_TRUE(schemaTableField.isPrimarykey());

  schemaTableField.setMulti(true);
  EXPECT_TRUE(schemaTableField.isMulti());

  schemaTableField.setFilter(true);
  EXPECT_TRUE(schemaTableField.isFilter());

  schemaTableField.setSearch(true);
  EXPECT_TRUE(schemaTableField.isSearch());

  schemaTableField.setAggregate(true);
  EXPECT_TRUE(schemaTableField.isAggregate());

  std::string field_name = "field_name";
  schemaTableField.setFieldName(field_name);
  EXPECT_EQ(field_name, schemaTableField.getFieldName());

  std::string outter_table = "outter_table";
  schemaTableField.setOuterTable(outter_table);
  EXPECT_EQ(outter_table, schemaTableField.getOuterTable());

  SchemaTableFieldType type = SchemaTableFieldType::INT64;
  schemaTableField.setType(type);
  EXPECT_EQ(type, schemaTableField.getType());

  schemaTableField.addIndex("index1");
  schemaTableField.addIndex("index2");
  EXPECT_EQ(2, schemaTableField.getIndexList().size());
}
