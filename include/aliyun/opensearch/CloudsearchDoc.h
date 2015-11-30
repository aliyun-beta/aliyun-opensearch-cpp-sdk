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

#ifndef OPENSEARCH_CLOUDSEARCHDOC_H_
#define OPENSEARCH_CLOUDSEARCHDOC_H_

#include <stdint.h>

#include <map>
#include <queue>
#include <string>
#include <vector>

namespace aliyun {
namespace opensearch {
class CloudsearchClient;
}  // namespace opensearch
}  // namespace aliyun

namespace aliyun {
namespace opensearch {

class CloudsearchDoc {
 public:
  typedef std::string string;

  static const string DOC_ADD;
  static const string DOC_REMOVE;
  static const string DOC_UPDATE;

  /**
   * push数据时API返回的正确的状态值。
   */
  static const string PUSH_RETURN_STATUS_OK;  // = "OK";

  /**
   * push数据时验证签名的方式。
   *
   * 如果此常量为1，且生成签名的query string中包含了items字段，则计算签名的时候items字段 将不被包含在内。否则，所有的字段将都要被计算签名。
   *
   */
  static const int SIGN_MODE = 1;

  /**
   * 在切割一个大数据块后push数据的频率。默认 5次/s。
   */
  static const int PUSH_FREQUENCE = 5;

  /**
   * POST一个文件，进行切割时的单请求的最大size。默认为2MB。
   */
  static const int PUSH_MAX_SIZE = 2 * 1024 * 1024;

  /**
   * Ha3Doc文件doc分割符。\x1E\n \x1E 对应ASCII=30
   */
  static const string HA_DOC_ITEM_SEPARATOR;  // = "\u001E";

  /**
   * Ha3Doc文件字段分割符 \x1F\n \x1F 对应ASCII=31
   */
  static const string HA_DOC_FIELD_SEPARATOR;  // = "\u001F";

  /**
   * Ha3Doc文件字段多值分割符。\x1D \x1D 对应ASCII=29
   */
  static const string HA_DOC_MULTI_VALUE_SEPARATOR;  // = "\u001D";

  /**
   * section weight标志符。\x1C \x1C 对应ASCII=28
   */
  static const string HA_DOC_SECTION_WEIGHT;  // = "\u001C";

  /**
   * 构造函数
   *
   * @param indexName 指定操作的索引名称。
   * @param client CloudsearchClient实例。
   */
  CloudsearchDoc(string indexName, CloudsearchClient& client);

  /**
   * 查看文档详情
   *
   * 根据doc id获取doc的详细信息。
   *
   * @param docId 指定的doc id。
   * @return 返回API返回的结果。
   * @throws IOException
   * @throws ClientProtocolException
   */
  string detail(string docId);

  /**
   * 添加文档
   *
   * 设置需要添加的属性名称和属性值，用于生成符合文档格式的数据，所有更新结束之后需要调用push(string tableName)方法
   *
   * @param fields 字段名和字段值的map
   * @throws JSONException
   */
  void add(const std::map<string, string>& fields);

  /**
   * 更新文档
   *
   * 设置需要更新的属性名称和属性值，用于生成符合文档格式的数据，所有更新结束之后需要调用push(string tableName)方法
   *
   * @param fields 字段名和字段值的map
   * @throws JSONException
   */
  void update(const std::map<string, string>& fields);

  /**
   * 删除文档
   *
   * 设置需要删除的属性名称和属性值，用于生成符合文档格式的数据，所有更新结束之后需要调用push(string tableName)方法
   *
   * @param fields 字段名和字段值的map
   * @throws JSONException
   */
  void remove(const std::map<string, string>& fields);

  /**
   * 执行文档变更操作(1)
   *
   * 针对文档的操作add、update和remove会生成符合文档格式的数据，通过调用此接口用户提交的文档变更才会真正生效。
   *
   * @param tableName 表名称
   * @return 返回的数据
   * @throws IOException
   * @throws ClientProtocolException
   */
  string push(string tableName);

  /**
   * 执行文档变更操作(2)
   *
   * 通过此接口可以直接将符合文档格式的数据直接推送到指定的表中
   *
   * @param docs 此docs为用户push的数据，此字段为json类型的字符串。
   * @param tableName 操作的表名。
   * @return 请求API并返回相应的结果。
   * @throws IOException
   * @throws ClientProtocolException
   */
  string push(string docs, string tableName);

  /**
   * 通过文件导入数据(1)
   *
   * 导入HA3 doc数据到指定的应用的指定表中
   *
   * 文件编码：UTF-8
   * 支持CMD: add, delete, update。如果给出的字段不是全部，add会在未给出的字段加默认值，
   * 覆盖原值；update只会更新给出的字段，未给出的不变。
   *
   * 文件分隔符：
   * <pre>
   * 编码--------------描述-----------------------显示形态--------------------
   * "\x1E\n"         每个doc的分隔符.             ^^(接换行符)
   * "\x1F\n"         每个字段key和value分隔.      ^_(接换行符)
   * "\x1D"           多值字段的分隔符.             ^]
   * </pre>
   * 示例：
   * <pre>
   * CMD=add^_
   * url=http://www.etao.com/index.html^_
   * title=一淘^_
   * body=xxxxx_xxxx^_
   * multi_value_feild=123^]1234^]12345^_
   * ^^
   * CMD=update^_
   * ...
   * </pre>
   * NOTE: 文件结尾的分隔符也必需为"^^\n"，最后一个换行符不能省略。
   *
   *
   * @param filePath 指定的文件路径。
   * @param tableName 指定push数据的表名。
   * @return 返回成功或者错误信息。
   * @throws JSONException
   */
  string pushHADocFile(string filePath, string tableName);

  /**
   * 通过文件导入数据(2)
   *
   * 导入HA3 doc数据到指定的应用的指定表中
   *
   * @param filePath 指定的文件路径。
   * @param tableName 指定push数据的表名。
   * @param offset 文档数据的偏移量，小于设定的offset行号的doc将被跳过
   * @return 返回成功或者错误信息。
   * @throws JSONException
   */
  string pushHADocFile(string filePath, string tableName, int64_t offset);

  /**
   * 检查发送频率限制。
   *
   * @param Queue<Long> timeLimitQueue
   */
  void timeLimitCheck(std::queue<long>& timeLimitQueue);

  /**
   * 获取上次请求的信息
   *
   * @return String
   */
  string getDebugInfo() const {
    return this->debugInfo_;
  }
 private:
  void operate(string cmd, const std::map<string, string>& fields);

  /**
   * 索引名称。
   */
  string indexName_;

  /**
   * CloudsearchClient实例。
   */
  CloudsearchClient* client_;

  /**
   * 请求的API的URI。
   */
  string path_;

  /**
   * 进行提交的数据
   */
  std::vector<string> requestArray_;

  /**
   * 调用client时发送的请求串信息
   */
  string debugInfo_;
};

}  // namespace opensearch
}  // namespace aliyun

#endif // OPENSEARCH_CLOUDSEARCHDOC_H_
