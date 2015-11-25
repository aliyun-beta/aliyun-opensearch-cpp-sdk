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

#ifndef OPENSEARCH_CLOUDSEARCHSEARCH_H_
#define OPENSEARCH_CLOUDSEARCHSEARCH_H_

#include <string>
#include <vector>
#include "CloudsearchClient.h"
#include "object/SearchTypeEnum.h"
#include "../utils/Any.h"

namespace aliyun {
namespace opensearch {

/**
 * opensearch 搜索接口。
 *
 * 此接口提供给用户通过简单的方式来生成搜索的语法，并提交进行搜索。
 *
 * 此接口生成的http 请求串的参数包含：query、client_id、index_name、fetch_fields、
 * first_formula_name、formula_name和summary等。
 *
 * example：
 * <code>
 * Map<std::string,utils::Any> param = new HashMap<std::string,utils::Any>();
 * List<std::string> indexList = new ArrayList<std::string>();
 * indexList.add("yourindexname");
 * search.addIndex(indexList);
 * search.setQuerystd::string("id:'1387936343940'");
 * search.setFormat("json");
 * std::string result = search.search();
 * </code>
 *
 */
class CloudsearchSearch {
 public:
  /**
   * 设定搜索结果集升降排序的标志。
   *
   * 根据sort的子句来设定搜索结果集的排序，"+"为升序，"-"为降序。
   *
   */
  static const std::string SORT_INCREASE; // = "+";
  static const std::string SORT_DECREASE; // = "-";

  static const std::string KEY_FORMAT; // = "format";
  static const std::string KEY_START; // = "start";
  static const std::string KEY_HITS; // = "hit";
  static const std::string KEY_RERANKSIZE; // = "rerank_size";
  static const std::string SEARCH_TYPE_SCAN; // = "scan";

  typedef utils::Any SummaryValue;

  typedef std::map<std::string, utils::Any> SummaryMap;

  typedef std::map<std::string, SummaryMap> StringSummaryMap;

  typedef object::SearchTypeEnum SearchTypeEnum;

  CloudsearchSearch(CloudsearchClient& client);

  /**
   * 执行搜索请求(1)
   *
   * @param opts 此参数如果被赋值，则会把此参数的内容分别赋给相应 的变量。此参数的值 可能有以下内容：
   *        query 指定的搜索查询串，可以为query=>'鲜花'，也可以为query=>"索引名:'鲜花'"。
   *        indexes 指定的搜索应用，可以为一个索引，也可以多个索引查询。
   *        fetch_fields 设定只返回某些字段的值。
   *        format 指定返回的数据格式，有json,xml和protobuf三种格式可选。
   *        formula_name 指定的精排表达式名称，此名称需在网站中设定。
   *        first_formula_name 指定的粗排表达式名称，此名称需在网站中指定。
   *        summary 指定summary字段一些标红、省略、截断等规则。
   *        start 指定搜索结果集的偏移量。默认值：0
   *        hits 指定返回结果集的数量。 默认值：20
   *        sort 指定排序规则。 默认值：“-”（降序）
   *        filter 指定通过某些条件过滤结果集。
   *        aggregate 指定统计类的信息。
   *        distinct 指定distinct排序。
   *        kvpair 指定的kvpair内容。
   *        qp     指定的查询分析规则名称。
   *        disable_qp 指定需要关闭的功能模块。
   *
   * @return std::string 返回搜索结果。
   * @throws IOException
   * @throws ClientProtocolException
   *
   */
  std::string search(SummaryMap& opts);

  /**
   * 执行搜索请求(2)
   *
   * @return std::string 返回搜索结果。
   * @throws IOException
   * @throws ClientProtocolException
   */
  std::string search();

  /**
   * 执行遍历搜索结果请求(1)
   *
   * @param opts 此参数如果被赋值，则会把此参数的内容分别赋给相应 的变量。此参数的值 可能有以下内容：
   *        query 指定的搜索查询串，可以为query=>'鲜花'，也可以为query=>"索引名:'鲜花'"。
   *        indexes 指定的搜索应用，可以为一个索引，也可以多个索引查询。
   *        fetch_fields 设定只返回某些字段的值。
   *        formula_name 指定的精排表达式名称，此名称需在网站中设定。
   *        first_formula_name 指定的粗排表达式名称，此名称需在网站中指定。
   *        summary 指定summary字段一些标红、省略、截断等规则。
   *        hits 指定返回结果集的数量。
   *        filter 指定通过某些条件过滤结果集。
   *        kvpair 指定的kvpair内容。
   *        qp     指定的查询分析规则名称。
   *        disable 指定需要关闭的功能模块。
   *        disable_qp 指定需要关闭的功能模块。
   * @return 搜索结果
   * @throws IOException
   * @throws UnknownHostException
   * @throws ClientProtocolException
   *
   */
  std::string scroll(SummaryMap& opts);


  /**
   * 执行遍历搜索结果请求(2)
   *
   * @return 搜索结果
   * @throws IOException
   * @throws UnknownHostException
   * @throws ClientProtocolException
   *
   */
  std::string scroll();

  /**
   * 添加一个应用列表来进行搜索
   *
   * @param indexes 应用名称或应用名称列表。
   */
  void setIndex(const std::vector<std::string> indexes) {
    this->indexes_ = indexes;
  }

  /**
   * 添加一个应用来进行搜索
   *
   * @param indexName 要搜索的应用名称。
   */
  void addIndex(std::string indexName);

  /**
   * 在当前搜索中去掉一个应用的搜索结果
   *
   * @param indexName 要移除的应用名称
   */
  void removeIndex(std::string indexName);

  /**
   * 获取当前请求中所有的应用名列表
   *
   * @return List<std::string> 返回当前搜索的所有应用列表。
   */
  std::vector<std::string>& getSearchIndexes() {
    return this->indexes_;
  }

  /**
   * 设置精排表达式名称(formula)
   *
   * 此表达式名称和结构需要在网站中已经设定，详情请浏览官网中的应用指定的表达式名称。
   *
   * @param formulaName 表达式名称。
   */
  void setFormulaName(std::string formulaName) {
    this->formulaName_ = formulaName;
  }

  /**
   * 获取当前设置的表达式名称(formula)
   *
   * @return std::string 返回当前设定的表达式名称。
   */
  std::string getFormulaName() {
    return this->formulaName_;
  }

  /**
   * 设置粗排表达式名称(first_formula)
   *
   * 此表达式名称和结构需要在网站中已经设定,详情请浏览官网中的应用指定的表达式名称。
   *
   * @param formulaName 表达式名称。
   */
  void setFirstFormulaName(std::string formulaName) {
    this->firstFormulaName_ = formulaName;
  }

  /**
   * 获取当前设置的粗排表达式名称(first_formula)
   *
   * @return std::string 返回当前设定的表达式名称。
   */
  std::string getFirstFormulaName() {
    return this->firstFormulaName_;
  }


  /**
   * 添加一条动态摘要(summary)信息(1)
   *
   * 增加了此内容后，fieldName字段可能会被截断、飘红等。
   *
   * @param fieldName 指定的生效的字段。此字段必需为可分词的text类型的字段。
   * @param len 指定结果集返回的词字段的字节长度，一个汉字为2个字节。
   * @param element 指定命中的query的标红标签，可以为em等。
   * @param ellipsis 指定用什么符号来标注未展示完的数据，例如“...”。
   * @param snippet 指定query命中几段summary内容。
   *
   * @return boolean 返回是否添加成功。
   */
  bool addSummary(std::string fieldName, int len, std::string element,
                  std::string ellipsis, int snippet);

  /**
   * 添加一条动态摘要(summary)信息(2)
   *
   * 增加了此内容后，fieldName字段可能会被截断、飘红等。
   *
   * @param fieldName 指定的生效的字段。此字段必需为可分词的text类型的字段。
   *
   * @return boolean 返回是否添加成功。
   */
  bool addSummary(std::string fieldName);

  /**
   * 添加一条动态摘要(summary)信息(3)
   *
   * 增加了此内容后，fieldName字段可能会被截断、飘红等。
   *
   * @param fieldName 指定的生效的字段。此字段必需为可分词的text类型的字段。
   * @param len 指定结果集返回的词字段的字节长度，一个汉字为2个字节。
   * @param ellipsis 指定用什么符号来标注未展示完的数据，例如“...”。
   * @param snippet 指定query命中几段summary内容。
   * @param elementPrefix 指定标签前缀。
   * @param elementPostfix 指定标签后缀。
   *
   * @return boolean 返回是否添加成功。
   */
  bool addSummary(std::string fieldName, int len, std::string ellipsis,
                  int snippet, std::string elementPrefix, std::string elementPostfix);

  /**
   * 获取当前所有设定的摘要信息(summary)
   *
   * @return Map 返回summary信息
   */
  const StringSummaryMap &getSummary() const {
    return this->summary_;
  }

  /**
   * 获取指定字段的摘要信息(summary)
   * 
   * @param fieldName 指定的字段名称。
   * 
   * @return SummaryMap 返回指定字段的summary信息。
   */
  SummaryMap getSummary(std::string fieldName);


  /**
   * 把摘要信息生成字符串并返回(summary)
   *
   * @return std::string 返回字符串的summary信息。
   */
  std::string getSummaryString();


  /**
   * 设置返回的数据格式名称(format)
   *
   * @param format 数据格式名称，有xml, json和protobuf 三种类型。默认值为：“xml”
   */
  void setFormat(std::string format) {
    configMap_[KEY_FORMAT] = format;
  }

  /**
   * 获取当前的数据格式名称(format)
   *
   * @return std::string 返回当前的数据格式名称。
   */
  std::string getFormat() const {
    SummaryMap::const_iterator it = configMap_.find(KEY_FORMAT);
    return it != configMap_.end() ? it->second.toString() : "";
  }

  /**
   * 设置返回结果的偏移量(start)
   *
   * @param start 偏移量。默认值为：0
   */
  void setStartHit(int start) {
    configMap_[KEY_START] = start;
  }

  /**
   * 获取返回结果的偏移量(start)
   *
   * @return int 返回当前设定的偏移量。
   */
  int getStartHit();

  /**
   * 设置当前返回结果集的文档个数(hit)
   *
   * @param hits 指定的doc个数。默认值为：20,取值范围：[0,500]
   */
  void setHits(int hits);


  /**
   * 获取当前设定的结果集的文档条数(hit)
   *
   * @return int 返回当前指定的doc个数。
   */
  int getHits();

  /**
   * 增加一个排序字段及排序方式(sort)
   *
   * @param field 需要排序的字段名称。
   * @param sortChar 排序方式，有升序“+”和降序“-”两种方式。默认值为“-”
   */
  void addSort(std::string field, std::string sortChar) {
    sort_[field] = sortChar;
  }

  /**
   * 增加一个排序字段(sort)
   *
   * @param field 指定排序的字段名称。
   */
  void addSort(std::string field) {
    this->addSort(field, SORT_DECREASE);
  }

  /**
   * 删除指定字段的排序(sort)
   *
   * @param field 指定的字段名称。
   */
  void removeSort(std::string field) {
    if (sort_.size() > 0 && sort_.find(field) != sort_.end()) {
      sort_.erase(field);
    }
  }

  /**
   * 获取排序信息(sort)
   *
   * @return 返回当前所有的排序字段及升降序方式。
   */
  std::map<std::string, std::string> getSort() {
    return sort_;
  }

  /**
   * 获取字符串类型的排序信息(sort)
   *
   * 把排序信息生成字符串并返回。
   *
   * @return std::string 返回字符串类型的排序规则。
   */
  std::string getSortString() const;

  /**
   * 增加一个自定义参数
   *
   * @param paramKey 参数名称。
   * @param paramValue 参数值。
   */
  void addCustomParam(std::string paramKey, std::string paramValue) {
    this->customParams_[paramKey] = paramValue;
  }

  /**
   * 获取自定义参数
   *
   * @return 返回自定义参数
   */
  std::map<std::string, std::string> getCustomParam() {
    return this->customParams_;
  }

  /**
   * 增加过滤规则(filter)(1)
   *
   * @param filter 过滤规则，例如fieldName >= 1。
   * @param operator 操作符，可以为 AND OR。默认为“AND”
   */
  void addFilter(std::string filter, std::string op);

  /**
   * 增加过滤规则(filter)(2)
   *
   * @param filter 过滤规则。
   */
  void addFilter(std::string filter) {
    this->addFilter(filter, "AND");
  }

  /**
   * 获取过滤规则(filter)
   *
   * @return std::string 返回字符串类型的过滤规则。
   */
  std::string getFilter() {
    return this->filter_;
  }

  /**
   * 添加统计信息(aggregate)相关参数(1)
   *
   * 一个关键词通常能命中数以万计的文档，用户不太可能浏览所有文档来获取信息。而用户感兴趣的
   * 可 能是一些统计类的信息，比如，查询“手机”这个关键词，想知道每个卖家所有商品中的最高价格。
   * 则可以按照卖家的user_id分组，统计每个小组中最大的price值，例如：
   * groupKey:user_id,aggFun:max(price)
   *
   * 相关wiki，请查询：
   * @link http://docs.aliyun.com/?spm=5176.2020520121.103.8.VQIcGd&tag=tun#/pub/opensearch/api-reference/query-clause&aggregate-clause
   *
   * @param groupKey 指定需要统计的字段名称。
   * @param aggFun 指定统计的方法。当前支持：count、max、min、sum等。
   * @param range 指定统计范围。
   * @param maxGroup 最大组个数。
   * @param aggFilter 指定过滤某些统计。
   * @param aggSamplerThresHold 指定抽样的伐值。
   * @param aggSamplerStep 指定抽样的步长。
   *
   * @return boolean 返回添加成功或失败。
   */
  bool addAggregate(std::string groupKey, std::string aggFun, std::string range,
                    std::string maxGroup, std::string aggFilter, std::string aggSamplerThresHold,
                    std::string aggSamplerStep);

  /**
   * 添加统计信息(aggregate)相关参数(2)
   *
   * @param groupKey 指定需要统计的字段名称。
   * @param aggFun 指定统计的方法。当前支持：count、max、min、sum等。
   *
   * @return boolean 返回添加成功或失败。
   */

  bool addAggregate(std::string groupKey, std::string aggFun) {
    return addAggregate(groupKey, aggFun, "", "", "", "", "");
  }

  /**
   * 获取用户设定的统计相关信息(aggregate)
   *
   * @return 返回用户设定的统计信息。
   */

  const std::vector<SummaryMap> &getAggregate() const {
    return this->aggregate_;
  }

  /**
   * 返回字符串类型的统计信息(aggregate)
   *
   * @return 返回字符串类型的统计信息。
   */
  std::string getAggregateString() const;

  /**
   * 添加聚合打散条件(distinct)(1)
   *
   * 例如：检索关键词“手机”共获得10个结果，分别为：doc1，doc2，doc3，doc4，doc5，doc6，
   * doc7，doc8，doc9，doc10。其中前三个属于用户A，doc4-doc6属于用户B，剩余四个属于
   * 用户C。如果前端每页仅展示5个商品，则用户C将没有展示的机会。但是如果按照user_id进行抽
   * 取，每轮抽取1个，抽取2次，并保留抽取剩余的结果，则可以获得以下文档排列顺序：doc1、
   * doc4、doc7、doc2、doc5、doc8、doc3、doc6、doc9、doc10。可以看出，通过distinct
   * 排序，各个用户的 商品都得到了展示机会，结果排序更趋于合理。
   *
   * @param key 为用户用于做distinct抽取的字段，该字段要求为可过滤字段。
   * @param distCount 为一次抽取的document数量，默认值为1。
   * @param distTimes 为抽取的次数，默认值为1。
   * @param reserved 为是否保留抽取之后剩余的结果，true为保留，false则丢弃，丢弃时totalHits的个数会减去被distinct而丢弃的个数，但这个结果不一定准确，默认为true。
   * @param distFilter 为过滤条件，被过滤的doc不参与distinct，只在后面的 排序中，这些被过滤的doc将和被distinct出来的第一组doc一起参与排序。默认是全部参与distinct。
   * @param updateTotalHit 当reserved为false时，设置update_total_hit为true，则最终total_hit会减去被distinct丢弃的的数目（不一定准确），为false则不减； 默认为false。
   * @param grade 指定档位划分阈值。
   *
   * @return 返回是否添加成功。
   */
  bool addDistinct(std::string key, int distCount, int distTimes,
                   std::string reserved, std::string distFilter, std::string updateTotalHit,
                   double grade);

  /**
   * 添加聚合打散条件(distinct)(2)
   *
   * @param key 为用户用于做distinct抽取的字段，该字段要求为可过滤字段。
   *
   * @return 返回是否添加成功。
   */
  bool addDistinct(std::string key) {
    return this->addDistinct(key, 0, 0, "", "", "", 0);
  }

  /**
   * 添加聚合打散条件(distinct)(3)
   *
   * @param key 为用户用于做distinct抽取的字段，该字段要求为可过滤字段。
   * @param distCount 为一次抽取的document数量，默认值为1。
   *
   * @return 返回是否添加成功。
   */
  bool addDistinct(std::string key, int distCount) {
    return this->addDistinct(key, distCount, 0, "", "", "", 0);
  }

  /**
   * 添加聚合打散条件(distinct)(4)
   *
   * @param key 为用户用于做distinct抽取的字段，该字段要求为可过滤字段。
   * @param distCount 为一次抽取的document数量，默认值为1。
   * @param distTimes 为抽取的次数，默认值为1。
   *
   * @return 返回是否添加成功。
   */
  bool addDistinct(std::string key, int distCount, int distTimes) {
    return this->addDistinct(key, distCount, distTimes, "", "", "", 0);
  }

  /**
   * 添加聚合打散条件(distinct)(5)
   *
   * @param key 为用户用于做distinct抽取的字段，该字段要求为可过滤字段。
   * @param distCount 为一次抽取的document数量，默认值为1。
   * @param distTimes 为抽取的次数，默认值为1。
   * @param reserved 为是否保留抽取之后剩余的结果，true为保留，false则丢弃，丢 弃时totalHits的个数会减去被distinct而丢弃的个数，但这个结果不一定准确，默认为true。
   * @return 返回是否添加成功。
   */
  bool addDistinct(std::string key, int distCount, int distTimes,
                   std::string reserved) {
    return this->addDistinct(key, distCount, distTimes, reserved, "", "", 0);
  }

  /**
   * 添加聚合打散条件(distinct)(6)
   *
   * @param key 为用户用于做distinct抽取的字段，该字段要求为可过滤字段。
   * @param distCount 为一次抽取的document数量，默认值为1。
   * @param distTimes 为抽取的次数，默认值为1。
   * @param reserved 为是否保留抽取之后剩余的结果，true为保留，false则丢弃，丢弃时totalHits的个数会减去被distinct而丢弃的个数，但这个结果不一定准确，默认为true。
   * @param distFilter 为过滤条件，被过滤的doc不参与distinct，只在后面的 排序中，这些被过滤的doc将和被distinct出来的第一组doc一起参与排序。默认是全部参与distinct。
   *
   * @return 返回是否添加成功。
   */
  bool addDistinct(std::string key, int distCount, int distTimes,
                   std::string reserved, std::string distFilter) {
    return this->addDistinct(key, distCount, distTimes, reserved, distFilter, "", 0);
  }

  /**
   * 添加聚合打散条件(distinct)(7)
   *
   * @param key 为用户用于做distinct抽取的字段，该字段要求为可过滤字段。
   * @param distCount 为一次抽取的document数量，默认值为1。
   * @param distTimes 为抽取的次数，默认值为1。
   * @param reserved 为是否保留抽取之后剩余的结果，true为保留，false则丢弃，丢弃时totalHits的个数会减去被distinct而丢弃的个数，但这个结果不一定准确，默认为true。
   * @param distFilter 为过滤条件，被过滤的doc不参与distinct，只在后面的 排序中，这些被过滤的doc将和被distinct出来的第一组doc一起参与排序。默认是全部参与distinct。
   * @param updateTotalHit 当reserved为false时，设置update_total_hit为true，则最终total_hit会减去被distinct丢弃的的数目（不一定准确），为false则不减； 默认为false。
   * @return 返回是否添加成功。
   */
  bool addDistinct(std::string key, int distCount, int distTimes,
                   std::string reserved, std::string distFilter, std::string updateTotalHit) {
    return this->addDistinct(key, distCount, distTimes, reserved, distFilter, updateTotalHit, 0);
  }

  /**
   * 删除某个字段的所有distinct信息
   *
   * @param distinctKey 要删除的dist key字段名称。
   */
  void removeDistinct(std::string distinctKey);

  /**
   * 获取所有的distinct信息
   *
   * @return 返回所有的distinct信息。
   */
  StringSummaryMap &getDistinct() {
    return this->distinct_;
  }

  /**
   * 获取string类型的所有的distinct信息
   *
   * @return 返回字符串类型的distinct信息。
   */
  std::string getDistinctString();

  /**
   * 设定指定索引字段范围的搜索关键词(query)
   *
   * 此query是查询必需的一部分，可以指定不同的索引名，并同时可指定多个查询及之间的关系
   * （AND, OR, ANDNOT, RANK）。
   *
   * 例如查询subject索引字段的query:“手机”，可以设置为 query=subject:'手机'。
   *
   * 上边例子如果查询price 在1000-2000之间的手机，其查询语句为： query=subject:'手机'
   * AND price:[1000,2000]
   *
   * NOTE: text类型索引在建立时做了分词，而string类型的索引则没有分词。
   *
   * @link http://docs.aliyun.com/?spm=5176.2020520121.103.8.VQIcGd&tag=tun#/pub/opensearch/api-reference/query-clause&query-clause
   *
   * @param query 设定搜索的查询语法。
   */
  void setQueryString(std::string query) {
    this->query_ = query;
  }

  /**
   * 获取当前指定的查询词内容(query)
   *
   * @return 返回当前设定的查询query子句内容。
   */
  std::string getQuery() {
    return this->query_;
  }

  /**
   * 设定当前的kvpair
   *
   * @param pair kvpair内容
   *
   */
  void setPair(std::string pair) {
    this->kvpair_ = pair;
  }

  /**
   * 获取当前的kvpair
   *
   * @return std::string 返回当前设定的kvpair。
   */
  std::string getPair() {
    return this->kvpair_;
  }

  /**
   * 设定rerank_size
   *
   * @param rerank_size 精排算分文档个数,默认值200
   */
  void setRerankSize(int rerank_size) {
    this->configMap_[KEY_RERANKSIZE] = rerank_size;
  }

  /**
   * 获取当前rerank_size
   *
   * rerankSize表示参与精排算分的文档个数，一般不用修改，使用默认值就能满足，不用设置,会自动使用默认值200
   *
   * @return int 当前设定的rerank_size
   */
  int getRerankSize() const;

  /**
   * 设置搜索返回的索引字段列表(fetch_fields)
   *
   * @param fields 结果集返回的字段。
   */
  void addFetchFields(std::vector<std::string> fields) {
    fetches_.insert(fetches_.end(), fields.begin(), fields.end());
  }

  /**
   * 添加某个字段到搜索结果字段列表中(fetch_fields)
   *
   * @param field 指定的字段名称。
   */
 void addFetchField(std::string field) {
    fetches_.push_back(field);
  }

  /**
   * 获取搜索结果包含的字段列表(fetch_fields)
   *
   * @return 返回指定返回字段的列表。
   */
  std::vector<std::string>& getFetchFields() {
    return this->fetches_;
  }

  /**
   * 添加一条查询分析规则(qp)
   *
   * @param qpName 查询分析规则名称
   */
  void addQpName(std::string qpName) {
    this->qp_.push_back(qpName);
  }

  /**
   * 添加一条查询分析规则(qp)
   *
   * @param qpNames 查询分析规则名称
   */
  void addQpNames(std::vector<std::string> qpNames) {
    this->qp_.insert(this->qp_.end(), qpNames.begin(), qpNames.end());
  }

  /**
   * 获取当前设置的查询分析规则(qp)
   *
   * @return 返回当前设置的所有查询分析规则名称
   */
  std::vector<std::string>& getQpNames() {
    return this->qp_;
  }

  /**
   * 关闭某些功能模块(disable)
   *
   * 有如下场景需要考虑：
   * 1、如果要关闭整个qp的功能，则指定disableValue="qp"。
   * 2、要指定某个索引关闭某个功能，则可以指定disableValue="qp:function_name:index_names",
   *   其中index_names可以用“|”分隔，可以为index_name1|index_name2...
   * 3、如果要关闭多个function可以用“,”分隔，例如：disableValue="qp:function_name1:index_name1,qp:function_name2:index_name1"
   *
   * qp有如下模块：
   * 1、spell_check: 检查用户查询串中的拼写错误，并给出纠错建议。
   * 2、term_weighting: 分析查询中每个词的重要程度，并将其量化成权重，权重较低的词可能不会参与召回。
   * 3、stop_word: 根据系统内置的停用词典过滤查询中无意义的词
   * 4、synonym: 根据系统提供的通用同义词库和语义模型，对查询串进行同义词扩展，以便扩大召回。
   *
   * example:
   * "qp" 标示关闭整个qp
   * "qp:spell_check" 标示关闭qp的拼音纠错功能。
   * "qp:stop_word:index_name1|index_name2" 标示关闭qp中index_name1和index_name2上的停用词功能。
   *
   * @param functionName 需要禁用的函数名称
   * @param value 待禁用函数的详细说明
   */
  void addDisableFunction(std::string functionName, std::string value) {
    this->disable_[functionName] =  value;
  }

  /**
   * 获取当前设置的所有禁用函数(disable)
   *
   * @return 返回当前设置的所有需要禁用的函数
   */
  std::string getDisableFunctions();

  /**
   * 关闭查询分析模块(qp)
   *
   * qp有如下模块：
   * 1、spell_check: 检查用户查询串中的拼写错误，并给出纠错建议。
   * 2、term_weighting: 分析查询中每个词的重要程度，并将其量化成权重，权重较低的词可能不会参与召回。
   * 3、stop_word: 根据系统内置的停用词典过滤查询中无意义的词
   * 4、synonym: 根据系统提供的通用同义词库和语义模型，对查询串进行同义词扩展，以便扩大召回。
   *
   * example:
   * 标示关闭整个qp
   * "qp:spell_check" 标示关闭qp的拼音纠错功能。
   * "qp:stop_word:index_name1|index_name2" 标示关闭qp中index_name1和index_name2上的停用词功能。
   * "qp:stop_word:index_name1|index_name2,synonym:index_name3" 标示关闭qp中index_name1和index_name2上的停用词功能,关闭index_name3上的同义词功能
   *
   * @param opts 关闭qp的细节配置参数
   */
  void disableQp(std::map<std::string, std::vector<std::string> >& opts);

  /**
   * 获取禁用查询分析的processor的字段配置
   *
   * @param indexes
   * @return
   */
  std::string getIndexInQp(std::vector<std::string> indexes);

  /**
   * 关闭整个查询分析模块(qp)
   *
   */
  void disableQp() {
    this->disable_["qp"] = std::string("");
  }

  /**
   * 设置scroll请求的有效期
   *
   * @param expire 指定的scroll请求有效期 默认 1m 表示一分钟，支持的时间单位包括：w=Week, d=Day, h=Hour, m=minute, s=second
   */
  void setScrollExpire(std::string expire) {
    this->scroll_ = expire;
  }

  /**
   * 设置scroll请求的有效期
   *
   * @param expire 指定的scroll请求有效期 单位毫秒
   */
  void setScrollExpire(int expire) {
    this->scroll_ = utils::StringUtils::ToString(expire);
  }

  /**
   * 获取设置的scroll请求有效期
   * @return std::string 设置的scroll请求有效期
   */
  std::string getScrollExpire() {
    return this->scroll_;
  }

  /**
   * 设置搜索请求类型(search_type)
   *
   * @param searchType 设置的搜索请求类型
   */
  void setSearchType(std::string searchType) {
    this->searchType_ = searchType;
  }

  /**
   * 获取设置的搜索请求类型(search_type)
   *
   * @return std::string 设置的搜索请求类型
   */
  std::string getSearchType() {
    return this->searchType_;
  }

  /**
   * 设置scroll请求的起始id
   *
   * @param scrollId scroll请求的起始id
   */
  void setScrollId(std::string scrollId) {
    this->scrollId_ = scrollId;
  }

  /**
   * 获取设置的scroll请求起始id
   *
   * @return 设置的scroll请求起始id
   */
 std::string getScrollId() {
    return this->scrollId_;
  }

  /**
   * 清空用户设置的所有搜索条件
   */
  void clear();


  /**
   * 发起请求
   *
   * 生成HTTP的请求串，并通过CloudsearchClient类向API服务发出请求并返回结果。
   * query参数中的query子句和config子句必需的，其它子句可选。
   *
   * @return 返回API返回的结果。
   * @throws IOException
   * @throws ClientProtocolException
   * @throws UnknownHostException
   */
  std::string call(SearchTypeEnum type);


  /**
   * 获取config子句
   *
   * 生成搜索的config子句并返回。
   *
   * @return 返回config子句的string内容。
   */
  std::string clauseConfig();


  /**
   * 添加一条自定义配置
   *
   * @param key 自定义配置项key
   * @param value 自定义配置项value
   */
  template <typename ValueType>
  void addCustomConfig(std::string key, ValueType value) {
    configMap_[key] = value;
  }

  /**
   * 移除自定义配置
   * @param key 指定配置项的key
   */
  void removeCustomConfig(std::string key) {
    configMap_.erase(key);
  }

  /**
   * 获取上次搜索请求的信息
   *
   * @return std::string 上次搜索请求的信息
   */
  std::string getDebugInfo() {
    return this->debugInfo_;
  }

 private:
  void initCustomConfigMap();

  void extract(SummaryMap &opts, SearchTypeEnum type);

  CloudsearchClient *client_;

  /**
   * 此次检索指定的应用名称。
   * 可以指定单个应用名称，也可以指定多个应用名称来进行搜索。
   */
  std::vector<std::string> indexes_;

  /**
   * 指定某些字段的一些summary展示规则。
   *
   * 这些字段必需为可分词的text类型的字段。
   *
   * 例如:
   * 指定title字段使用动态摘要，则 summary_field=title
   * 指定title长度为50：summary_len=50
   * 指定title飘红标签：summary_element=em
   * 指定title省略符号：summary_ellipsis=...
   * 指定summary缩略段落个数：summary_snippet=1
   *
   * 那么当前的字段值为：
   * <code>
   * Map<std::string, utils::Any> summary = new HashMap<std::string, utils::Any>();
   * summary.put("summary_field", "title");
   * summary.put("summary_len", 50);
   * summary.put("summary_element", "em");
   * summary.put("summary_ellipsis", "...");
   * summary.put("summary_snippet", 1);
   *
   * Map<std::string, Map<std::string, utils::Any>> summaries = new HashMap<std::string, Map<std::string, utils::Any>>();
   * summaries.put("title", summary);
   *
   * </code>
   */
  StringSummaryMap summary_;

  /**
   * configs
   */
  SummaryMap configMap_;

  /**
   * sorting rules
   */
  std::map<std::string, std::string> sort_;

  std::string filter_;

  std::map<std::string, std::string> customParams_;

  std::vector<SummaryMap> aggregate_;

  std::map<std::string, SummaryMap> distinct_;

  std::vector<std::string> fetches_;

  std::string query_;

  /**
   * 指定精排表达式名称，表达式名称和结构在网站中指定。
   *
   * 用户需要现在网站管理后台中设定好当前请求的应用的精排表达式名称和表达式，在此只需设定表
   * 达式的名称即可。
   */
  std::string formulaName_;


  /**
   * 指定粗排表达式名称，表达式名称和结构在网站中指定。
   *
   * 用户需要现在网站管理后台中设定好当前请求的应用的精排表达式名称和表达式，在此只需设定表
   * 达式的名称即可。
   */
  std::string firstFormulaName_;

  std::string path_;

  std::string kvpair_;

  std::string debugInfo_;

  std::vector<std::string> qp_;

  SummaryMap disable_;

  std::string scroll_;

  std::string searchType_;

  std::string scrollId_;
};

}  // namespace opensearch
}  // namespace aliyun

#endif  // OPENSEARCH_CLOUDSEARCHSEARCH_H_
