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

#include "aliyun/utils/date.h"

#include <string.h>

namespace aliyun {
namespace utils {

const char* Date::kMonthName[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

Date::Date()
    : time_(0) {
  ::memset(&tm_, 0, sizeof(struct tm));
}

Date::Date(time_t t)
    : time_(t) {
  ::memcpy(&tm_, ::localtime(&time_), sizeof(struct tm));
}

Date::Date(struct tm* tm) {
  tm_ = *tm;
  time_ = ::mktime(&tm_);
}

// fill as a specific local date-time
Date::Date(int year, int mon, int day, int hour, int min, int sec) {
  //  care follow members of `struct tm`:
  //    tm_year   The number of years since 1900.
  //    tm_mon    The number of months since January, in the range 0 to 11.
  //    tm_wday   The number of days since Sunday, in the range 0 to 6.
  tm_.tm_year = year - 1900;
  tm_.tm_mon = mon - 1;
  tm_.tm_mday = day;
  tm_.tm_hour = hour;
  tm_.tm_min = min;
  tm_.tm_sec = sec;
  time_ = ::mktime(&tm_);
}

Date Date::currentLocalDate() {
  Date d;
  d.time_ = time(NULL);
  d.tm_ = *::localtime(&d.time_);
  return d;
}

Date Date::currentUtcDate() {
  Date d;
  d.time_ = time(NULL);
  d.tm_ = *::gmtime(&d.time_);
  return d;
}

std::string Date::format(const char* fmt, const struct tm* t) {
  char buffer[128];
  ::strftime(buffer, sizeof(buffer), fmt, t);
  return buffer;
}

std::string Date::toString() const {
  return invalid() ? "invalid-date" : format("%Y-%m-%d %H:%M:%S");
}

std::string Date::format(const char* fmt) const {
  return format(fmt, &tm_);
}

}  // namespace utils
}  // namespace aliyun
