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

TEST(SchemaTableField, ctor) {
  SchemaTableField field;
  EXPECT_EQ(field.isDisplay(), true);
  EXPECT_EQ(field.isPrimarykey(), false);
  EXPECT_EQ(field.isMulti(), false);
  EXPECT_EQ(field.isFilter(), false);
  EXPECT_EQ(field.isSearch(), false);
  EXPECT_EQ(field.isAggregate(), false);
}

TEST(SchemaTableField, addIndex) {
  SchemaTableField field;
  field.addIndex("index1");
  field.addIndex("index2");
  field.addIndex("index3");
  EXPECT_EQ(3, field.getIndexList().size());
}

TEST(SchemaTable, all) {
  std::string name = "table";
  SchemaTable table;
  SchemaTableField field;

  table.setTableName(name);
  table.addField(field);

  EXPECT_EQ(table.getTableName(), name);
  EXPECT_EQ(table.isMasterTable(), false);
  EXPECT_EQ(table.getFieldList().size(), 1);
}
