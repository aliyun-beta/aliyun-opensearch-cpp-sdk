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

#include "aliyun/reader/JsonReader.h"
#include "aliyun/utils/StringUtils.h"

#ifdef TRACE
#include <stdio.h> // for debug
#include <stdlib.h>
int gTraceCounter = 0;
#define trace printf
#define TRACE_FUNC do { \
  if(++gTraceCounter > 800) exit(-1); \
  else { \
    ::printf("%2d %s\n", gTraceCounter, __FUNCTION__); \
  } \
} while(0)
#else
#define TRACE_FUNC
#define trace(...)
#endif

namespace aliyun {

namespace reader {

std::map<char, char> JsonReader::escapes;

static int __init__ = JsonReader::staticInitializer();

int JsonReader::staticInitializer() {
  escapes['\\'] = '\\';
  escapes['/'] = '/';
  escapes['"'] = '"';
  escapes['t'] = '\t';
  escapes['n'] = '\n';
  escapes['r'] = '\r';
  escapes['b'] = '\b';
  escapes['f'] = '\f';
  trace("%s\n", __FUNCTION__);
  return 0xfeeded00;
}

JsonReader::Token JsonReader::readJson(string baseKey) {
  TRACE_FUNC;
  while (s_ && ::isspace(*s_)) {
    s_++;
  }
  char ch = *s_;
  if (ch == '\0')
    return END_TOKEN;
  s_++;
  switch (ch) {
    case '{':
      token_ = OBJECT_BEGIN_TOKEN;
      processObject(baseKey);
      break;
    case '}':
      token_ = OBJECT_END_TOKEN;
      break;
    case '[':
      token_ = ARRAY_BEGIN_TOKEN;
      if (*s_ == '"') {  // for string array
        processList(baseKey);
      } else {  // for object array
        processArray(baseKey);
      }
      break;
    case ']':
      token_ = ARRAY_END_TOKEN;
      break;
    case '"':
      token_ = STRING_TOKEN;
      processString();
      break;
    case ',':
      token_ = COMMA_TOKEN;
      break;
    case ':':
      token_ = COLON_TOKEN;
      break;
    case '\t':
      token_ = TABLE_TOKEN;
      s_ += 3;
      break;
    case '\n':
      token_ = NEWLINE_TOKEN;
      s_ += 3;
      break;
    case '\f':
      token_ = FEED_TOKEN;
      s_ += 4;
      break;
    default:
      s_--;
      if (::isdigit(*s_) || *s_ == '-') {
        token_ = NUMBER_TOKEN;
        processNumber();
      }
  }
  return token_;
}

void JsonReader::processObject(string baseKey) {
  TRACE_FUNC;
  readJson(baseKey);
  string key = baseKey + '.' + stringBuffer_;
  while (token_ != OBJECT_END_TOKEN && *s_) {
    readJson(key);  // separator
    if (token_ != OBJECT_END_TOKEN) {
      Token t = readJson(key);
      if (t == STRING_TOKEN || t == NUMBER_TOKEN) {
        trace("put [%s] [%s]\n", key.c_str(), stringBuffer_.c_str());
        map_[key] = stringBuffer_;
      } else if (t == TABLE_TOKEN) {
        trace("put [%s] [%s]\n", key.c_str(), "true");
        map_[key] = "true";
      } else if (t == FEED_TOKEN) {
        trace("put [%s] [%s]\n", key.c_str(), "false");
        map_[key] = "false";
      }

      if (readJson(key) == COMMA_TOKEN) {  // separator ?
        readJson(key);
        key = baseKey + '.' + stringBuffer_;
      }
    }
  }
  if (token_ != OBJECT_END_TOKEN)
    throw JsonException("object unclosed");
}

void JsonReader::processList(string baseKey) {
  TRACE_FUNC;
  using aliyun::utils::StringUtils::ToString;
  readJson(baseKey);
  int index = 0;
  while (token_ != ARRAY_END_TOKEN && *s_) {
    string key = trimFromLast(baseKey, ".") + "[" + ToString(index++) + "]";
    trace("put [%s] [%s]\n", key.c_str(), stringBuffer_.c_str());
    map_[key] = stringBuffer_;
    if (readJson(baseKey) == COMMA_TOKEN) {  // separator ?
      stringBuffer_.clear();
      readJson(baseKey);
    }
  }
  if (token_ != ARRAY_END_TOKEN)
    throw JsonException("list unclosed");
  trace("put [%s.Length] [%d]\n", baseKey.c_str(), index);
  map_[trimFromLast(baseKey, ".") + ".Length"] = ToString(index);
}

void JsonReader::processArray(string baseKey) {
  TRACE_FUNC;
  using aliyun::utils::StringUtils::ToString;
  int index = 0;
  string preKey = trimFromLast(baseKey, ".");
  string key = preKey + "[" + ToString(index) + "]";

  readJson(key);
  while (token_ != ARRAY_END_TOKEN && *s_) {
    trace("put [%s.Length] [%d]\n", preKey.c_str(), index);
    map_[preKey + ".Length"] = ToString(index + 1);

    if (token_ == STRING_TOKEN) {
      trace("put [%s] [%s]\n", key.c_str(), stringBuffer_.c_str());
      map_[key] = stringBuffer_;
    }

    if (readJson(baseKey) == COMMA_TOKEN) {
      key = preKey + "[" + ToString(++index) + "]";
      readJson(key);
    }
  }
  if (token_ != ARRAY_END_TOKEN)
    throw JsonException("array unclosed");
}

void JsonReader::processNumber() {
  TRACE_FUNC;
  stringBuffer_.clear();
  if ('-' == *s_) {
    addChar();
  }
  addDigits();
  if ('.' == *s_) {
    addChar();
    addDigits();
  }
  if ('e' == *s_ || 'E' == *s_) {
    addChar();
    if ('+' == *s_ || '-' == *s_) {
      addChar();
    }
    addDigits();
  }
  trace("took %s\n", stringBuffer_.c_str());
  // took a number to string buffer.
}

void JsonReader::processString() {
  TRACE_FUNC;
  stringBuffer_.clear();
  while (*s_ != '"' && *s_) {
    if (*s_ == '\\') {  // escape sequence.
      s_++;
      if (escapes.find(*s_) != escapes.end()) {
        stringBuffer_.push_back(escapes[*s_]);
        s_++;
      }
    } else {
      addChar();
    }
  }
  if (*s_ != '\"')
    throw JsonException("string unclosed");
  trace("took %s\n", stringBuffer_.c_str());
  s_++;  // skip '"'
}

void JsonReader::addDigits() {
  while (::isdigit(*s_)) {
    addChar();
  }
}

void JsonReader::addChar() {
  stringBuffer_.push_back(*s_++);
}

std::string JsonReader::trimFromLast(string str, string stripString) {
  string::size_type pos = str.find_last_of(stripString);
  if (pos != string::npos) {
    return str.substr(0, pos);
  } else {
    return str;
  }
}

std::map<std::string, std::string> JsonReader::read(string response,
                                                    string endpoint) {
#ifdef TRACE
  gTraceCounter = 0;
#endif
  s_ = response.c_str();
  readJson(endpoint);
  return map_;
}

}  // namespace reader

}  // namespace aliyun
