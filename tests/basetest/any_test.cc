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

#include <string>
#include <vector>

#include "aliyun/utils/any.h"
#include "aliyun/utils/string_utils.h"

using std::string;
using aliyun::utils::Any;
using aliyun::utils::AnyCast;
using aliyun::utils::BadAnyCast;
using aliyun::utils::StringUtils::ToString;

struct Foo {
  int id_;
  static int count_;
  Foo() : id_(++count_) { ::printf("Foo %d created!\n", id_); }
  Foo(const Foo&) : id_(++count_) { ::printf("Foo %d created!\n", id_); }
  ~Foo() { ::printf("Foo %d destoryed!\n", id_); }

  string toString() const {
    return "{id:" + ToString(id_) + "}";
  }
};

std::ostream& operator<<(std::ostream& out, const Foo& rhs) {
  out << rhs.toString();
  return out;
}

struct Bar {};

BEGIN_ALIYUN_TOSTRING_DECLARE
template <>
string ToString<const Bar&>(const Bar& bar) {
  return "Bar-" + ToString(&bar);
}
END_ALIYUN_TOSTRING_DECLARE

int Foo::count_ = 0;

TEST(AnyTest, testCtor) {
  // for build-in types
  Any a1 = 1234;
  Any a2(3.1415);
  Any a3 = static_cast<void*>(0);
  // Any aa = "fail";  // by default, gcc deal string literal as char[]
  Any a4 = static_cast<const char*>("str");

  // copy ctor
  Any aa1(a1);
  Any aa2 = a1;
}

TEST(AnyTest, testEmpty) {
  Any a;
  EXPECT_TRUE(a.empty());
}

TEST(AnyTest, testType) {
  Any a;
  Any a1 = 123;
  Any a2 = string("string");

  EXPECT_EQ(typeid(void), a.type());
  EXPECT_EQ(typeid(int), a1.type());
  EXPECT_EQ(typeid(string), a2.type());
}

TEST(AnyTest, testAnyCast) {
  int i = 123456;
  Any a = i;
  int origin = AnyCast<int>(a);  // expect no exception

  Foo foo;
  Any afoo(foo);
  Foo aorg = AnyCast<Foo>(afoo);  // from const Foo&

  // cover BadAnyCast
  try {
    Any a = 123;
    Foo f = AnyCast<Foo>(a);
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    EXPECT_EQ(typeid(BadAnyCast), typeid(e));
  }
}

TEST(AnyTest, testSwap) {
  int i = 123;
  string s = "foo";

  Any a = 123;
  Any b = string("foo");
  EXPECT_EQ(typeid(int), a.type());
  EXPECT_EQ(typeid(string), b.type());

  a.swap(b);
  EXPECT_EQ(typeid(int), b.type());
  EXPECT_EQ(typeid(string), a.type());

  EXPECT_EQ(i, AnyCast<int>(b));
  EXPECT_EQ(s, AnyCast<string>(a));
}

TEST(AnyTest, testAssignOperator) {
  int i = 1234;
  string s = "string is better";

  Any a = i;
  Any b = s;

  // assign from Any type
  a = b;
  EXPECT_EQ(a.type(), b.type());
  EXPECT_EQ(s, AnyCast<string>(a));

  // assign from another type
  a = Foo();
  EXPECT_EQ(typeid(Foo), a.type());
}
