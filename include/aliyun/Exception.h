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

#ifndef ALIYUN_EXCEPTION_H_
#define ALIYUN_EXCEPTION_H_

#include <stdlib.h>

#ifdef __GNUC__
#include <execinfo.h>
#endif

#include <exception>
#include <string>

namespace aliyun {

class Exception : public std::exception {
 public:
  explicit Exception(std::string what)
      : what_(what) {
    fillStackTrace();
  }

  virtual ~Exception() throw() {
  }

  virtual const char* what() const throw() {
    return what_.c_str();
  }

  const char* stackTrace() const throw() {
    return stack_.c_str();
  }

 private:
  // TODO(xu): add backtrace support for other platform
  void fillStackTrace() {
#ifdef __GNUC__
    const int len = 200;
    void* buffer[len];
    int nptrs = ::backtrace(buffer, len);
    char** symbols = ::backtrace_symbols(buffer, nptrs);
    if (symbols) {
      for (int i = 0; i < nptrs; ++i) {
        // TODO(xu): demangle funcion name with abi::__cxa_demangle
        stack_.append(symbols[i]);
        stack_.push_back('\n');
      }
      ::free(symbols);
    }
#endif
  }

  std::string what_;
  std::string stack_;
};

}  // namespace aliyun

#endif  // ALIYUN_EXCEPTION_H_
