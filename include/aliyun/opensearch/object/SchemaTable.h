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

#ifndef OPENSEARCH_OBJECT_SCHEMATABLE_H_
#define OPENSEARCH_OBJECT_SCHEMATABLE_H_

#include <string>
#include <vector>

#include "SchemaTableField.h"

namespace aliyun {
namespace opensearch {
namespace object {

class SchemaTable {
 public:
  SchemaTable();

  void addField(SchemaTableField schemaTableField);

  const std::vector<SchemaTableField>& getFieldList() const {
    return fieldList_;
  }

  void setFieldList(const std::vector<SchemaTableField>& fieldList) {
    this->fieldList_ = fieldList;
  }

  bool isMasterTable() const {
    return masterTable_;
  }

  void setMasterTable(bool masterTable) {
    this->masterTable_ = masterTable;
  }

  const std::string& getTableName() const {
    return tableName_;
  }

  void setTableName(std::string tableName) {
    this->tableName_ = tableName;
  }

 private:
  std::string tableName_;  // schema的表的名称
  bool masterTable_;  // 是否是主表
  std::vector<SchemaTableField> fieldList_;  // schema的属性
};

}  // namespace object
}  // namespace opensearch
}  // namespace aliyun

#endif  // OPENSEARCH_OBJECT_SCHEMATABLE_H_
