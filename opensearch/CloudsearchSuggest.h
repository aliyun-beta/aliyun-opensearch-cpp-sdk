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

#ifndef OPENSEARCH_CLOUDSEARCHSUGGEST_H_
#define OPENSEARCH_CLOUDSEARCHSUGGEST_H_

#include <string>

#include "CloudsearchClient.h"

namespace aliyun {
namespace opensearch {

class CloudsearchSuggest {
 public:
  typedef std::string String;

  CloudsearchSuggest();

  CloudsearchSuggest(String indexName, String suggestName, CloudsearchClient* client);

  ~CloudsearchSuggest();

 private:
  /**
   * CloudsearchClient实例
   */
  CloudsearchClient* client_;

  /**
   * 索引名称
   */
  String indexName;

  /**
   * 下拉提示名称
   */
  String suggestName;

  /**
   * 返回下拉结果条数
   */
  int hit;

  /**
   * 搜索内容
   */
  String query;

  /**
   * 请求API的部分path。
   */
  String path;

  /*
   * 调试信息
   * 用户上次调用的请求串
   */
  String debugInfo;
};

}  // namespace opensearch
}  // namespace aliyun

#endif  // OPENSEARCH_CLOUDSEARCHSUGGEST_H_
