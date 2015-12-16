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

#ifndef ALIYUN_OPENSEARCH_OBJECT_SCHEMA_TABLE_FIELD_TYPE_H_
#define ALIYUN_OPENSEARCH_OBJECT_SCHEMA_TABLE_FIELD_TYPE_H_

#include <string.h>
#include <string>

namespace aliyun {
namespace opensearch {
namespace object {

class SchemaTableFieldType {
 public:
// INT8("INT8", "INT"), UINT8("UINT8", "INT"),
// INT16("INT16", "INT"), UINT16("UINT16", "INT"),
// INT32("INT32", "INT"), UINT32("INT32", "INT"),
// INT64("INT64", "INT"), UINT64("INT64", "INT"),
// TEXT("TEXT", "TEXT"), STRING("STRING", "TEXT"),
// FLOAT("FLOAT", "FLOAT"), DOUBLE("DOUBLE","FLOAT");

// a bit of tricks to implement above mapping
#define COLUMNS(selector) \
        selector(INVALID, INVALID), \
        selector(INT8, INT), selector(UINT8, INT), \
        selector(INT16, INT), selector(UINT16, INT), \
        selector(INT32, INT), selector(UINT32, INT), \
        selector(INT64, INT), selector(UINT64, INT), \
        selector(TEXT, TEXT), selector(STRING, TEXT), \
        selector(FLOAT, FLOAT), selector(DOUBLE, FLOAT)

#define select1st(e1, e2) e1
#define select2nd(e1, e2) e2
#define string1st(e1, e2) #e1
#define string2nd(e1, e2) #e2

  enum Value {
    COLUMNS(select1st),
    INT  // dummy
  };
  static const int kMaxValue = DOUBLE;

  SchemaTableFieldType(Value v = INVALID);

  SchemaTableFieldType(std::string type, std::string bigType);

  std::string getTypeName() const;

#define IMPLEMENT_COMPARE(op) \
  bool operator op (const SchemaTableFieldType& rhs) const { \
    return this->value_ op rhs.value_; \
  }

  IMPLEMENT_COMPARE(==)

#undef IMPLEMENT_COMPARE

  std::string getBigTypeName() const {
    return bigTypeNames()[value_];
  }

  int getBigType() const {
    static int bigTypes[] = { COLUMNS(select2nd), };
    return bigTypes[value_];
  }

 private:
  static const char** typeNames() {
    static const char* names[] = { COLUMNS(string1st) };
    return names;
  }

  static const char** bigTypeNames() {
    static const char* names[] = { COLUMNS(string2nd) };
    return names;
  }

  Value value_;
};

#undef COLUMNS
#undef select1st
#undef select2nd
#undef string1st
#undef string2nd

}  // namespace object
}  // namespace opensearch
}  // namespace aliyun

#endif  // ALIYUN_OPENSEARCH_OBJECT_SCHEMA_TABLE_FIELD_TYPE_H_
