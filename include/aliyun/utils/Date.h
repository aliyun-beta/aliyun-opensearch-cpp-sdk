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

#ifndef CORE_UTILS_DATE_H_
#define CORE_UTILS_DATE_H_

#include <time.h>
#include <string>

namespace aliyun {

namespace utils {

// depends on time.h, no guarantee to ensure thread safety
class Date {
 public:

  const static int kNumMonth = 12;
  static const char* kMonthName[];

  Date();

  Date(time_t t);

  Date(struct tm* tm);

  Date(int year, int mon, int day, int hour, int min, int sec);

  static Date currentLocalDate();

  static Date currentUtcDate();

  inline bool invalid() const {
    return time_ == 0;
  }

#define IMPLEMENT_COMPARE(op) \
  inline bool operator op (const Date& rhs) const { \
    return time_ op rhs.time_; \
  }

  IMPLEMENT_COMPARE(==)
  IMPLEMENT_COMPARE(!=)
  IMPLEMENT_COMPARE(<=)
  IMPLEMENT_COMPARE(>=)
  IMPLEMENT_COMPARE(<)
  IMPLEMENT_COMPARE(>)

#undef IMPLEMENT_COMPARE

  std::string toString() const;

  std::string format(const char* fmt) const;

  // return raw handle
  const struct tm* c_tm() const {
    return &tm_;
  }

 private:

  static std::string format(const char* fmt, const struct tm* t);

  time_t time_;
  struct tm tm_;  // cache local time represent.
};

}  // namespace utils

}  // namespace aliyun

#endif  // CORE_UTILS_DATE_H_
