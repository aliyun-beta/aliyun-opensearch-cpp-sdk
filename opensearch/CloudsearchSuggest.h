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
  CloudsearchSuggest(std::string indexName, std::string suggestName, CloudsearchClient *client);

  /**
   * 获取应用名称
   *
   * @return 应用名称
   */
  std::string getIndexName();

  /**
   * 获取下拉提示名称
   *
   * @return 下拉提示名称
   */
  std::string getSuggestName();

  /**
   * 设置获取的下拉提示结果条数
   *
   * @param hit 设置获取的下拉提示结果条数 默认值：10
   */
  void setHit(int hit);

  /**
   * 获取设置的下拉提示结果条数
   *
   * @return int 获取设置的下拉提示结果条数
   */
  int getHit();

  /**
   * 设置查询词
   *
   * @param query 查询词
   */
  void setQuery(std::string query);

  /**
   * 获取查询词
   *
   * @return std::string 查询词
   */
  std::string getQuery();


  /**
   * 发起查询请求获取查询结果
   *
   * @return String 下拉提示查询结果
   * @throws ClientProtocolException
   * @throws IOException
   */
  std::string search();

  /**
   * 获取上次请求的信息
   *
   * @return std::string
   */
  std::string getDebugInfo();

 private:
  /**
   * CloudsearchClient实例
   */
  CloudsearchClient *client_;

  /**
   * 索引名称
   */
  std::string indexName_;

  /**
   * 下拉提示名称
   */
  std::string suggestName_;

  /**
   * 下拉结果条数
   */
  int hit_;

  /**
   * 搜索内容
   */
  std::string query_;

  /**
   * 请求API的部分path。
   */
  std::string path_;

  /*
   * 调试信息
   * 用户上次调用的请求串
   */
  std::string debugInfo_;
};

}  // namespace opensearch
}  // namespace aliyun

#endif  // OPENSEARCH_CLOUDSEARCHSUGGEST_H_
