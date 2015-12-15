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

#include "aliyun/utils/ParameterHelper.h"

#include <apr_md5.h>
#include <apr_uuid.h>
#include <cstring>

#include "aliyun/utils/Base64Helper.h"

#ifndef ARRAYSIZE
#define ARRAYSIZE(a) sizeof(a)/sizeof(a[0])
#endif

namespace aliyun {

namespace utils {

// "2014-12-22T10:33:40Z"
static const char* FORMAT_ISO8601 = "%Y-%m-%dT%H:%M:%SZ";

// "Wed, 16 Jan 2013 19:01:18 GMT"
static const char* FORMAT_RFC2616 = "%a, %d %b %Y %H:%M:%S GMT";

std::string ParameterHelper::md5hex(std::string str) {
  byte md[APR_MD5_DIGESTSIZE] = { 0 };
  string result;

  const byte* data = reinterpret_cast<const byte*>(str.c_str());
  size_t len = static_cast<size_t>(str.length());

  apr_md5(md, data, len);

  for (int i = 0; i < APR_MD5_DIGESTSIZE; i++) {
    char tmp[4];
    ::snprintf(tmp, sizeof(tmp), "%02x", md[i]);
    result += tmp;
  }
  return result;
}

std::string ParameterHelper::md5Sum(std::string str) {
  byte md[APR_MD5_DIGESTSIZE] = { 0 };
  apr_md5(md, reinterpret_cast<const byte*>(str.c_str()),
          static_cast<size_t>(str.length()));
  return Base64Helper::encode(md, APR_MD5_DIGESTSIZE);
}

ParameterHelper::byte *ParameterHelper::md5(const byte* data, size_t len) {
  static byte md[APR_MD5_DIGESTSIZE] = { 0 };
  apr_md5(md, data, len);
  return md;
}

std::string ParameterHelper::getUUID() {
  apr_uuid_t uuid;
  char buffer[APR_UUID_FORMATTED_LENGTH + 1];

  apr_uuid_get(&uuid);
  apr_uuid_format(buffer, &uuid);
  return buffer;
}

std::string ParameterHelper::getISO8601Date(Date date) {
  if (date.invalid()) {
    date = Date::currentUtcDate();  // current
  }
  return date.format(FORMAT_ISO8601);
}

std::string ParameterHelper::getRFC2616Date(Date date) {
  if (date.invalid()) {
    date = Date::currentUtcDate();
  }
  return date.format(FORMAT_RFC2616);
}

Date ParameterHelper::parse(string strDate) {
  if (strDate.length() == 0) {
    return Date::currentUtcDate();
  }
  try {
    return parseISO8601(strDate);
  } catch (ParseDateException& e) {
    return parseRFC2616(strDate);
  }
}

Date ParameterHelper::parseISO8601(string strDate) {
  if (strDate.length() == 0) {
    return Date::currentUtcDate();
  }
  // expect format: "2014-12-22T10:33:40Z"
  int year, mon, day, hour, min, sec;
  year = mon = day = hour = min = sec = 0;
  if (::sscanf(strDate.c_str(), "%d-%d-%dT%d:%d:%dZ", &year, &mon, &day, &hour,
               &min, &sec) != 6) {
    throw ParseDateException("unmatch format ISO8601");
  }
  return Date(year, mon, day, hour, min, sec);
}

Date ParameterHelper::parseRFC2616(string strDate) {
  if (strDate.length() == 0) {
    return Date::currentUtcDate();
  }

  // expect format: "Wed, 16 Jan 2013 19:01:18 GMT"
  char month[8] = "";
  char wkday[8] = "";  // for week day
  char tzone[8] = "";  // for time zone
  int year, day, hour, min, sec;
  year = day = hour = min = sec = 0;

  // limit string token length, to avoid stack overflow !
  if (::sscanf(strDate.c_str(), "%4s %d %3s %d %d:%d:%d %3s", wkday, &day,
               month, &year, &hour, &min, &sec, tzone) != 8) {
    throw ParseDateException("unmatch format RFC2616");
  }
  wkday[3] = '\0';
  // ::printf("%s: %s %s %s\n", __FUNCTION__, wkday, month, tzone); // debug

  // check month string
  int mon = Date::kNumMonth;
  for (int i = 0; i < Date::kNumMonth; i++) {
    if (::strncmp(month, Date::kMonthName[i], sizeof(month)) == 0) {
      mon = i + 1;
      break;
    }
  }
  if (mon == Date::kNumMonth) {
    throw ParseDateException("unmatch format RFC2616, bad month");
  }

  // TODO(xu): optional, check week day and time zone string.

  return Date(year, mon, day, hour, min, sec);
}

}  // namespace utils

}  // namespace aliyun
