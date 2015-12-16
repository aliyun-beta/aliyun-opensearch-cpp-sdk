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

#ifndef ALIYUN_OPENSEARCH_CLOUDSEARCH_INDEX_H_
#define ALIYUN_OPENSEARCH_CLOUDSEARCH_INDEX_H_

#include <map>
#include <string>

#include "aliyun/opensearch/cloudsearch_client.h"

namespace aliyun {
namespace opensearch {

class CloudsearchIndex {
 public:
  typedef CloudsearchClient& ClientRef;

  /**
   * 构造函数
   *
   * @param indexName 索引的名称
   * @param client 提交请求的client
   */
  CloudsearchIndex(std::string indexName, ClientRef client);

  /**
   * 根据模板名称创建应用(1)
   *
   * @param templateName 模板名称
   * @param opts 包含应用的备注信息,具体包括：index_des-应用的描述信息,package_id-该应用选择的套餐
   * @return 返回api返回的正确或错误的结果
   * @throws IOException
   * @throws ClientProtocolException
   */
  std::string createByTemplateName(std::string templateName,
                                   const std::map<std::string,
                                                  std::string> &opts);

  /**
   * 根据模板名称创建应用(2)
   *
   * @param templateName 模板名称
   * @return 返回api返回的正确或错误的结果
   * @throws IOException
   * @throws ClientProtocolException
   */
  std::string createByTemplateName(std::string templateName);


  /**
   * 更新应用的名称和备注信息
   *
   * @param toIndexName 应用名称
   * @param opts 相关参数
   * @return 返回api返回的正确或错误的结果
   * @throws JSONException
   * @throws IOException
   * @throws ClientProtocolException
   */
  std::string rename(std::string toIndexName,
                     const std::map<std::string, std::string> &opts);


  /**
   * 删除当前的应用
   *
   * @return 返回API返回的正确或错误的结果
   * @throws IOException
   * @throws ClientProtocolException
   */
  std::string remove();
  inline std::string deleteIt() { return remove(); }

  /**
   * 查看当前应用的状态
   *
   * @return 返回API返回的正确或错误的结果
   * @throws IOException
   * @throws ClientProtocolException
   */
  std::string status();

  /**
   * 列出所有应用
   *
   * @param page 开始的页码
   * @param pageSize 获取的记录数
   * @return 返回API返回的正确或错误的结果
   * @throws IOException
   * @throws ClientProtocolException
   */
  std::string listIndexes(int page, int pageSize);

  /**
   * 创建一条导入数据或重建索引的任务
   *
   * 如果operate为import，则需要指定tableName，反之如果operate为build，则不需指定。
   * 例如： 如果想全量导入某个表的数据且导入完毕后重建索引，则可以调用如下：
   * <code>
   * createTask("import", "my_table", true);
   * </code>
   *
   * 如果只想全量导入某个表的数据但不建索引。 <code>
   * createTask("import", "my_table", false);
   * </code>
   *
   * 如果只想重建索引： <code>
   * createTask("build");
   * </code>
   *
   * 或
   *
   * <code>
   * createTask("build", "", false);
   * </code>
   *
   * @param operate 操作符，包含"build"、"import"，build为重建索引，import为数据导入。
   * @param tableName 指定的表的名称，此表必须已经配置了数据源，如果为build则此字段无效。
   * @param needBuild 指定是否重建索引。
   * @return 返回api的结果。
   * @throws ClientProtocolException
   * @throws IOException
   * @donotgenetatedoc
   */
  std::string createTask(std::string operate, std::string tableName,
                         bool needBuild);

  /**
   * 创建一条重建索引的任务
   *
   * @return 返回api返回的结果。
   * @throws ClientProtocolException
   * @throws IOException
   * @donotgenetatedoc
   */
  std::string createBuildTask();

  /**
   * 创建一条数据导入的任务
   *
   * @param tableName 指定的表的名称，此表必须配置了数据源。
   * @param needBuild 指定是否重建索引。
   * @donotgenetatedoc
   * @return 返回api返回的结果。
   * @throws ClientProtocolException
   * @throws IOException
   */
  std::string createImportTask(std::string tableName, bool needBuild);

  /**
   * 获取错误信息
   *
   * @param page 开始页数
   * @param pageSize 每页的记录数
   * @return API返回的错误信息
   * @throws IOException
   * @throws ClientProtocolException
   */
  std::string getErrorMessage(int page, int pageSize);

  inline std::string getIndexName() const { return indexName_; }

  inline std::string getDebugInfo() const { return debugInfo_; }

 private:
  CloudsearchClient *client_;

  std::string indexName_;

  std::string path_;

  std::string debugInfo_;
};

}  // namespace opensearch
}  // namespace aliyun

#endif  // ALIYUN_OPENSEARCH_CLOUDSEARCH_INDEX_H_
