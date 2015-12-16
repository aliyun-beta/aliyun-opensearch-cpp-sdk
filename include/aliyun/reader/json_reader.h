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

#ifndef ALIYUN_READER_JSON_READER_H_
#define ALIYUN_READER_JSON_READER_H_

#include <map>
#include <string>

#include "aliyun/exception.h"
#include "aliyun/reader/reader.h"

namespace aliyun {
namespace reader {

class JsonException : public Exception {
 public:
  explicit JsonException(std::string msg)
      : Exception(msg) {
  }
};

class JsonReader : public Reader {
  static std::map<char, char> escapes;

 public:
  typedef std::string string;

  std::map<string, string> read(string response, string endpoint);

 private:
  enum Token {
    INIT_TOKEN,
    ARRAY_BEGIN_TOKEN,
    ARRAY_END_TOKEN,
    OBJECT_BEGIN_TOKEN,
    OBJECT_END_TOKEN,
    STRING_TOKEN,
    NUMBER_TOKEN,
    BOOL_TOKEN,
    COMMA_TOKEN,
    COLON_TOKEN,
    TABLE_TOKEN,
    NEWLINE_TOKEN,
    FEED_TOKEN,
    END_TOKEN
  };

  Token readJson(string baseKey);

  void processObject(string baseKey);

  void processList(string baseKey);

  void processArray(string baseKey);

  void processNumber();

  void processString();

  void addDigits();

  void addChar();

  static string trimFromLast(string str, string stripString);

  void reset();

 private:
  Token token_;
  const char* s_;
  string stringBuffer_;
  std::map<string, string> map_;
};

}  // namespace reader
}  // namespace aliyun

#endif  // ALIYUN_READER_JSON_READER_H_
