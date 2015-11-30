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
#include <stdio.h>
#include <stdlib.h>
#include <apr_general.h>
#include <curl/curl.h>
#include "aliyun/utils/details/GlobalInitializer.h"

namespace aliyun {
namespace utils {
namespace details {

// for apr global init/cleanup
class AprGlobalWrapper {
 public:
  AprGlobalWrapper() {
    ::printf("%s apr_initialize\n", __FUNCTION__);
    if (apr_initialize() != APR_SUCCESS) {
      ::fprintf(stderr, "apr_initialize fail");
      ::exit(-1);
    }
  }

  ~AprGlobalWrapper() {
    ::printf("%s apr_terminate\n", __FUNCTION__);
    apr_terminate();
  }

  static AprGlobalWrapper* instance() {
    static AprGlobalWrapper stub;
    return &stub;
  }
};

// for curl global init/cleanup
class CurlGlobalWrapper {
 public:
  CurlGlobalWrapper() {
    ::printf("%s curl_global_init\n", __FUNCTION__);
    curl_global_init(CURL_GLOBAL_ALL);
  }

  ~CurlGlobalWrapper() {
    ::printf("%s curl_global_cleanup\n", __FUNCTION__);
    curl_global_cleanup();
  }

  static CurlGlobalWrapper* instance() {
    static CurlGlobalWrapper stub;
    return &stub;
  }
};

// other wrapper/initializer add to here.

int GlobalInitializer::initialize() {
  ::printf("%s\n", __FUNCTION__);
  // singleton like.
  AprGlobalWrapper::instance();
  CurlGlobalWrapper::instance();
  return 0x900dfee1;
}

}  // namespace details
}  // namespace utils
}  // namespace aliyun
