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

#ifndef ALIYUN_OPENSEARCH_OBJECT_SCHEMATABLEFIELD_H_
#define ALIYUN_OPENSEARCH_OBJECT_SCHEMATABLEFIELD_H_

#include <string>
#include <vector>

#include "SchemaTableFieldType.h"

namespace aliyun {
namespace opensearch {
namespace object {

class SchemaTableField {
 public:
  SchemaTableField();

  /**
   * 添加一个索引项
   * @param indexStr
   */
  void addIndex(std::string indexStr);

  bool isAggregate() const {
    return aggregate_;
  }

  void setAggregate(bool aggregate) {
    this->aggregate_ = aggregate;
  }

  bool isDisplay() const {
    return display_;
  }

  void setDisplay(bool display) {
    this->display_ = display;
  }

  const std::string& getFieldName() const {
    return fieldName_;
  }

  void setFieldName(std::string fieldName) {
    this->fieldName_ = fieldName;
  }

  bool isFilter() const {
    return filter_;
  }

  void setFilter(bool filter) {
    this->filter_ = filter;
  }

  const std::vector<std::string>& getIndexList() const {
    return indexList_;
  }

  bool isMulti() const {
    return multi_;
  }

  void setMulti(bool multi) {
    this->multi_ = multi;
  }

  const std::string& getOuterTable() const {
    return outerTable_;
  }

  void setOuterTable(std::string outerTable) {
    this->outerTable_ = outerTable;
  }

  bool isPrimarykey() const {
    return primarykey_;
  }

  void setPrimarykey(bool primarykey) {
    this->primarykey_ = primarykey;
  }

  bool isSearch() const {
    return search_;
  }

  void setSearch(bool search) {
    this->search_ = search;
  }

  const SchemaTableFieldType& getType() const {
    return type_;
  }

  void setType(const SchemaTableFieldType& type) {
    this->type_ = type;
  }

 private:
  std::string fieldName_;  // 字段的名称
  SchemaTableFieldType type_;  // 字段的类型
  bool primarykey_;  // 是否是主键
  bool multi_;  // 是否是多值
  bool filter_;  // 是否可以过滤
  bool search_;  // 是否可以搜索
  bool display_;  // = true;  // 是否可以展示，默认可展示
  bool aggregate_;  // 是否可以聚合

  // 如果定义了此值，则这个字段会和指定的表的primary key关联
  std::string outerTable_;

  // 指定字段索引字段名称，如果未指定，TEXT类型默认为default，
  // 其他默认为字段名称。可以为单值或者数组。
  std::vector<std::string> indexList_;
};

}  // namespace object
}  // namespace opensearch
}  // namespace aliyun

#endif  // ALIYUN_OPENSEARCH_OBJECT_SCHEMATABLEFIELD_H_
