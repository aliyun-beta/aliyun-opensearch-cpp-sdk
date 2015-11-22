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

#include "SchemaTable.h"

#include <regex>

#include "SchemaTableFieldType.h"

namespace aliyun {
namespace opensearch {

SchemaTable::SchemaTable()
    : masterTable_(false) {
}

void SchemaTable::addField(SchemaTableField schemaTableField) {
  if (schemaTableField.isPrimarykey()) {
    schemaTableField.setSearch(true);
    schemaTableField.addIndex(schemaTableField.getFieldName());
    if (schemaTableField.getType() == SchemaTableFieldType::TEXT) {
      schemaTableField.setFilter(false);
      schemaTableField.setAggregate(false);
    } else if (schemaTableField.getType() == SchemaTableFieldType::STRING) {
      schemaTableField.setFilter(false);
    }
  }

  // for TEXT
  if (schemaTableField.getType() == SchemaTableFieldType::TEXT) {
    schemaTableField.setAggregate(false);
    schemaTableField.setFilter(false);
    if (schemaTableField.getIndexList().size() == 0) {
      schemaTableField.addIndex("default");
    } else {
      for (auto& indexStr : schemaTableField.getIndexList()) {
        std::regex pattern("^[_a-zA-Z][a-zA-Z0-9_]*");
        if (!std::regex_match(indexStr, pattern)) {
          return;
        }
      }
    }
  }

  // for STRING
  if (schemaTableField.getType() == SchemaTableFieldType::STRING) {
    if (!schemaTableField.isMulti()) {
      schemaTableField.setAggregate(false);
    }
    schemaTableField.setFilter(false);
    if (schemaTableField.getIndexList().size() == 0) {
      schemaTableField.addIndex(schemaTableField.getFieldName());
    } else {
      for (auto& indexStr : schemaTableField.getIndexList()) {
        std::regex pattern("^[_a-zA-Z][a-zA-Z0-9_]*");
        if (!std::regex_match(indexStr, pattern)) {
          return;
        }
      }
    }
  }

  // for FLOAT, DOUBLE, INT
  if (schemaTableField.getType() == SchemaTableFieldType::FLOAT
      || schemaTableField.getType() == SchemaTableFieldType::DOUBLE
      || schemaTableField.getType().getBigType()
          == SchemaTableFieldType(SchemaTableFieldType::INT8).getBigType()) {
    schemaTableField.addIndex(schemaTableField.getFieldName());
    if (!schemaTableField.isMulti()) {
      schemaTableField.setFilter(false);
    }
  }
  this->fieldList_.push_back(schemaTableField);
}
}  // namespace opensearch
}  // namespace aliyun
