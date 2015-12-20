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
#include "aliyun/opensearch/object/schema_table.h"

using aliyun::opensearch::object::SchemaTable;
using aliyun::opensearch::object::SchemaTableField;
using aliyun::opensearch::object::SchemaTableFieldType;

TEST(SchemaTableTest, testCtor) {
  SchemaTable table;
  EXPECT_EQ("", table.getTableName());
  EXPECT_FALSE(table.isMasterTable());
  EXPECT_EQ(0, table.getFieldList().size());
}

TEST(SchemaTableTest, testGettersAndSetters) {
  SchemaTable table;
  table.setMasterTable(true);
  EXPECT_TRUE(table.isMasterTable());

  table.setTableName("table-name");
  EXPECT_EQ("table-name", table.getTableName());
}


TEST(SchemaTableTest, testAddField) {
  SchemaTable table;
  SchemaTableField field;

  field.setPrimarykey(true);
  table.addField(field);
  EXPECT_EQ(1, table.getFieldList().size());
  EXPECT_TRUE(table.getFieldList()[0].isPrimarykey());

  field.setType(SchemaTableFieldType::TEXT);
  table.addField(field);
  EXPECT_EQ(2, table.getFieldList().size());

  field.setType(SchemaTableFieldType::STRING);
  table.addField(field);
  EXPECT_EQ(3, table.getFieldList().size());

  field.setType(SchemaTableFieldType::FLOAT);
  table.addField(field);
  EXPECT_EQ(4, table.getFieldList().size());

  SchemaTableField field1;
  field1.addIndex("index1");
  field1.setType(SchemaTableFieldType::TEXT);
  table.addField(field1);
  EXPECT_EQ(5, table.getFieldList().size());

  field1.setType(SchemaTableFieldType::STRING);
  table.addField(field1);
  EXPECT_EQ(6, table.getFieldList().size());
}
