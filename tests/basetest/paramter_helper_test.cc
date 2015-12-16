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
#include "aliyun/utils/parameter_helper.h"

using aliyun::utils::Date;
using aliyun::utils::ParseDateException;
using aliyun::utils::ParameterHelper;

#define ARRAYSIZE(a) sizeof(a)/sizeof(a[0])

TEST(MD5, test) {
    const char *test[] = {
        "",
        "a",
        "abc",
        "message digest",
        "abcdefghijklmnopqrstuvwxyz",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
    };

    const char *ret[] = {
        "d41d8cd98f00b204e9800998ecf8427e",
        "0cc175b9c0f1b6a831c399e269772661",
        "900150983cd24fb0d6963f7d28e17f72",
        "f96b697d7cb7938d525a2f31aaf161d0",
        "c3fcd3d76192e4007dfb496cca67e13b",
        "d174ab98d277d9f5a5611c2c9f419d9f",
        "57edf4a22be3c955ac49da2e2107b67a",
    };

  for (int i = 0; i < sizeof(test) / sizeof(test[0]); i++) {
    std::string md5str = ParameterHelper::md5hex(test[i]);
    EXPECT_TRUE(md5str == ret[i]);
  }
}

TEST(UUID, test) {
  int nRounds = 10000;
  std::set<std::string> dict;

  for (int i = 0; i < nRounds; i++) {
    dict.insert(ParameterHelper::getUUID());
  }
  EXPECT_TRUE(dict.size() == nRounds);
}

TEST(DateISO8601, test) {
  std::string dateStr = "2014-12-22T10:33:40Z";
  Date expect(2014, 12, 22, 10, 33, 40);
  try {
    // cover ParameterHelper::parseISO8601
    Date parsed = ParameterHelper::parseISO8601(dateStr);
    EXPECT_TRUE(parsed == expect);

    // cover ParameterHelper::getISO8601Date
    EXPECT_TRUE(ParameterHelper::getISO8601Date(parsed) == dateStr);
  } catch (ParseDateException& e) {
    std::cout << "ParseException:" << e.what() << std::endl;
  } catch (std::exception& e) {
    std::cout << "exception: " << e.what() << std::endl;
  } catch (...) {
    std::cout << "unknow exception" << std::endl;
  }
}

TEST(DateRFC2616, test) {
  std::string dateStr = "Wed, 16 Jan 2013 19:01:18 GMT";
  Date expect(2013, 1, 16, 19, 1, 18);
  try {
    // cover ParameterHelper::parseRFC2616
    Date parsed = ParameterHelper::parseRFC2616(dateStr);
    EXPECT_TRUE(parsed == expect);

    std::cout << ParameterHelper::getRFC2616Date(parsed) << std::endl;
    // cover ParameterHelper::getRFC2616Date
    EXPECT_TRUE(ParameterHelper::getRFC2616Date(parsed) == dateStr);
  } catch (ParseDateException& e) {
    std::cout << "ParseException:" << e.what() << std::endl;
  } catch (std::exception& e) {
    std::cout << "exception: " << e.what() << std::endl;
  } catch (...) {
    std::cout << "unknow exception" << std::endl;
  }
}

void testDate(std::string str) {
  std::cout << "testDate(" << str << "): ";
  try {
    // cover ParameterHelper::parse
    Date parsed = ParameterHelper::parse(str);
    std::cout << parsed.toString() << " PASS!\n";
  } catch (ParseDateException& e) {
    std::cout << "ParseException:" << e.what() << std::endl;
  } catch (std::exception& e) {
    std::cout << "exception: " << e.what() << std::endl;
  } catch (...) {
    std::cout << "unknow exception" << std::endl;
  }
}

TEST(Date, sidecases) {
  testDate("");
  testDate("2014-12-22T10:33:40Z");
  testDate("2014-12-22 10:33:40Z");
  testDate("2014-12-22 10:33:40 ");
  testDate("2014-12-22T10:33:40 ");

  testDate("Wed, 16 Jan 2013 19:01:18 GMT");
  testDate("I-want-to-broken-stack-buffer, 16 Jan 2013 19:01:18 GMT");
  testDate("Wed, 16 I-want-to-broken-stack-buffer 2013 19:01:18 GMT");
  testDate("Wed, 16 Jan 2013 19:01:18 I-want-to-broken-stack-buffer");
  testDate("Wed,#16 Jan 2013 19:01:18 GMT");
  testDate("Wed, 16#Jan 2013 19:01:18 GMT");
  testDate("Wed, 16 Jan#2013 19:01:18 GMT");
  testDate("Wed, 16 Jan 2013#19:01:18 GMT");
  testDate("Wed, 16 Jan 2013 19:01:18#GMT");
  testDate("Wed, 16 Jan 2013 19:01:18 #MT");
  testDate("Wed, 16 Jan 2013 19:01:18 G#T");
  testDate("Wed, 16 Jan 2013 19:01:18 GM#");
}
