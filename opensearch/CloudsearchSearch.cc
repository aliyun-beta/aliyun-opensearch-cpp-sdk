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

#include "CloudsearchSearch.h"

namespace aliyun {
namespace opensearch {

using std::string;
using utils::AnyCast;

const string CloudsearchSearch::SORT_INCREASE = "+";
const string CloudsearchSearch::SORT_DECREASE = "-";

const string CloudsearchSearch::KEY_FORMAT = "format";
const string CloudsearchSearch::KEY_START = "start";
const string CloudsearchSearch::KEY_HITS = "hit";
const string CloudsearchSearch::KEY_RERANKSIZE = "rerank_size";
const string CloudsearchSearch::SEARCH_TYPE_SCAN = "scan";

void CloudsearchSearch::initCustomConfigMap() {
  this->configMap_[KEY_FORMAT] = string("xml");
  this->configMap_[KEY_START] = string("0");
  this->configMap_[KEY_HITS] = string("20");
  this->configMap_[KEY_RERANKSIZE] = string("200");
}

CloudsearchSearch::CloudsearchSearch(CloudsearchClient *client) {
  this->client_ = client;
  this->initCustomConfigMap();
}

std::string CloudsearchSearch::search(SummaryMap& opts) {
  this->extract(opts, SearchTypeEnum::SEARCH);
  return this->call(SearchTypeEnum::SEARCH);
}

void CloudsearchSearch::extract(SummaryMap& opts, SearchTypeEnum type) {
  if (opts.size() > 0) {
    SummaryMap::iterator pos = opts.find("config");
    if (pos != opts.end()) {
      try {
        SummaryMap configMap = AnyCast<SummaryMap>(pos->second);
        for (SummaryMap::iterator it = configMap.begin(); it != configMap.end();
            ++it) {
          this->addCustomConfig(it->first, it->second);
        }
      } catch (utils::BadAnyCast &e) {
      }
    }

    pos = opts.find("query");
    if (pos != opts.end()) {
      this->setQueryString(AnyCast<string>(pos->second));
    }
    pos = opts.find("filter");
    if (pos != opts.end()) {
      this->addFilter(AnyCast<string>(pos->second));
    }
    pos = opts.find("knpair");
    if (pos != opts.end()) {
      this->setPair(AnyCast<string>(pos->second));
    }
    pos = opts.find("indexes");
    if (pos != opts.end()) {
      this->setIndex(AnyCast<std::vector<string> >(pos->second));
    }
    pos = opts.find("fetch_field");
    if (pos != opts.end()) {
      this->addFetchField(AnyCast<string>(pos->second));
    }
    pos = opts.find("format");
    if (pos != opts.end()) {
      this->setFormat(AnyCast<string>(pos->second));
    }
    pos = opts.find("hits");
    if (pos != opts.end()) {
      this->setHits(AnyCast<int>(pos->second));
    }
    pos = opts.find("start");
    if (pos != opts.end()) {
      this->setStartHit(AnyCast<int>(pos->second));
    }
    pos = opts.find("formula_name");
    if (pos != opts.end()) {
      this->setFormulaName(AnyCast<string>(pos->second));
    }
    pos = opts.find("first_formula_name");
    if (pos != opts.end()) {
      this->setFormulaName(AnyCast<string>(pos->second));
    }
    pos = opts.find("summary");
    if (pos != opts.end()) {
      this->summary_ = AnyCast<StringSummaryMap>(pos->second);
    }
    pos = opts.find("qp");
    if (pos != opts.end()) {
      this->addQpName(AnyCast<string>(pos->second));
    }
    pos = opts.find("disable_qp");
    if (pos != opts.end()) {
      this->disableQp(
          AnyCast<std::map<string, std::vector<string> > &>(pos->second));
    }

    if (type == SearchTypeEnum::SEARCH) {
      pos = opts.find("sort");
      if (pos != opts.end()) {
        this->sort_ = AnyCast<std::map<string, string> >(pos->second);
      }
      pos = opts.find("aggregate");
      if (pos != opts.end()) {
        this->aggregate_ = AnyCast<std::vector<SummaryMap> >(pos->second);
      }
      pos = opts.find("distinct");
      if (pos != opts.end()) {
        this->distinct_ = AnyCast<StringSummaryMap>(pos->second);
      }
    } else if (type == SearchTypeEnum::SCROLL) {
      pos = opts.find("scroll");
      if (pos != opts.end()) {
        this->setScrollExpire(AnyCast<string>(pos->second));
      }
      pos = opts.find("search_type");
      if (pos != opts.end()) {
        this->setSearchType(AnyCast<string>(pos->second));
      }
      pos = opts.find("scroll_id");
      if (pos != opts.end()) {
        this->setScrollId(AnyCast<string>(pos->second));
      }
    }
  }
}

void CloudsearchSearch::addIndex(std::string indexName) {
  this->indexes_.push_back(indexName);
}

void CloudsearchSearch::removeIndex(std::string indexName) {
  std::vector<std::string> buffer;
  for (int i = 0; i < this->indexes_.size(); ++i) {
    if (this->indexes_[i] != indexName) {
      buffer.push_back(this->indexes_[i]);
    }
  }
  this->indexes_.swap(buffer);
}

bool CloudsearchSearch::addSummary(std::string fieldName, int len,
                                   std::string element, std::string ellipsis,
                                   int snippet) {
  if (fieldName.length() == 0) {
    return false;
  }

  SummaryMap summaryMap;
  summaryMap["summary_field"] = fieldName;
  if (len != 0) {
    summaryMap["summary_len"] = len;
  }
  if (element.length() != 0) {
    summaryMap["summary_element"] = element;
  }
  if (ellipsis.length() != 0) {
    summaryMap["summary_ellipsis"] = ellipsis;
  }
  if (snippet != 0) {
    summaryMap["summary_snippet"] = snippet;
  }
  this->summary_[fieldName] = summaryMap;
  return true;
}

bool CloudsearchSearch::addSummary(std::string fieldName) {
  return this->addSummary(fieldName, 0, "", "", 0);
}

bool CloudsearchSearch::addSummary(std::string fieldName, int len,
                                   std::string ellipsis, int snippet,
                                   std::string elementPrefix,
                                   std::string elementPostfix) {
  if (fieldName.length() == 0) {
    return false;
  }
  SummaryMap summaryMap;
  summaryMap["summary_field"] = fieldName;
  summaryMap["summary_len"] = len;
  summaryMap["summary_ellipsis"] = ellipsis;
  summaryMap["summary_ellipsis"] = ellipsis;
  summaryMap["summary_snippet"] = snippet;
  summaryMap["summary_element_prefix"] = elementPrefix;
  summaryMap["summary_element_postfix"] = elementPostfix;

  this->summary_[fieldName] = summaryMap;
  return true;
}

CloudsearchSearch::SummaryMap CloudsearchSearch::getSummary(
    std::string fieldName) {
  StringSummaryMap::iterator pos = this->summary_.begin();
  if (pos != this->summary_.end()) {
    return pos->second;
  }
  return SummaryMap();
}

static std::string ToString(CloudsearchSearch::SummaryMap &summaryMap) {
  string str;
  for (CloudsearchSearch::SummaryMap::const_iterator it = summaryMap.begin();
      it != summaryMap.end(); ++it) {
    str += ',' + it->first + ':' + it->second.toString();
  }
  return str.substr(1);
}

static std::string ToString(const CloudsearchSearch::SummaryMap &summaryMap) {
  return ToString(const_cast<CloudsearchSearch::SummaryMap &>(summaryMap));
}

static std::string ToString(
    CloudsearchSearch::StringSummaryMap &stringSummaryMap) {
  string str;
  if (stringSummaryMap.size() > 0) {
    for (CloudsearchSearch::StringSummaryMap::const_iterator it =
        stringSummaryMap.begin(); it != stringSummaryMap.end(); ++it) {
      string sub;
      for (CloudsearchSearch::SummaryMap::const_iterator jt =
          it->second.begin(); jt != it->second.end(); ++jt) {
        sub += ',' + jt->first + ':' + jt->second.toString();
      }
      str += ';' + sub.substr(1);
    }
  }
  return str.substr(1);
}

static std::string ToString(
    const CloudsearchSearch::StringSummaryMap &stringSummaryMap) {
  return ToString(
      const_cast<CloudsearchSearch::StringSummaryMap &>(stringSummaryMap));
}

std::string CloudsearchSearch::getSummaryString() {
  return ToString(this->summary_);
}

bool CloudsearchSearch::addAggregate(std::string groupKey, std::string aggFun,
                                     std::string range, std::string maxGroup,
                                     std::string aggFilter,
                                     std::string aggSamplerThresHold,
                                     std::string aggSamplerStep) {
  if (groupKey.length() == 0 || aggFun.length() == 0) {
    return false;
  }

  SummaryMap aggregate;
  aggregate["group_key"] = groupKey;
  aggregate["agg_fun"] = aggFun;

  if (range.length() != 0) {
    aggregate["range"] = range;
  }

  if (maxGroup.length() != 0) {
    aggregate["max_group"] = maxGroup;
  }

  if (aggFilter.length() != 0) {
    aggregate["agg_filter"] = aggFilter;
  }

  if (aggSamplerThresHold.length() != 0) {
    aggregate["agg_sampler_threshold"] = aggSamplerThresHold;
  }

  if (aggSamplerStep.length() != 0) {
    aggregate["agg_sampler_step"] = aggSamplerStep;
  }
  this->aggregate_.push_back(aggregate);
  return true;
}

std::string CloudsearchSearch::getAggregateString() const {
  std::string str;
  const std::vector<SummaryMap> &aggs = this->getAggregate();
  if (aggs.size() > 0) {
    for (int i = 0; i < aggs.size(); i++) {
      str += ';' + ToString(aggs[i]);
    }
    return str.substr(1);
  }

  return str;
}

std::string CloudsearchSearch::getSortString() const {
  std::string sortStr;
  if (sort_.size() > 0) {
    for (std::map<std::string, std::string>::const_iterator it = sort_.begin();
        it != sort_.end(); ++it) {
      // FIXME(xu): leak separator ?
      sortStr += ';' + it->second + it->first;
    }
    return sortStr.substr(1);
  }
  return sortStr;
}

int CloudsearchSearch::getHits() {
  try {
    SummaryMap::iterator it = configMap_.find(KEY_HITS);
    if (it != configMap_.end()) {
      return utils::AnyCast<int>(it->second);
    }
    throw Exception("KeyNotFoundException: " + KEY_START);
  } catch (utils::BadAnyCast &e) {
    // ignore bad cast
  }
  return 20;
}

void CloudsearchSearch::setHits(int hits) {
  if (hits < 0) {
    hits = 0;
  }
  configMap_[KEY_HITS] = hits;
}

int CloudsearchSearch::getStartHit() {
  try {
    SummaryMap::iterator it = configMap_.find(KEY_START);
    if (it != configMap_.end()) {
      return utils::AnyCast<int>(it->second);
    }
    throw Exception("KeyNotFoundException: " + KEY_START);
  } catch (utils::BadAnyCast &e) {
    // ignore bad cast
  }
  return 0;
}

bool CloudsearchSearch::addDistinct(std::string key, int distCount,
                                    int distTimes, std::string reserved,
                                    std::string distFilter,
                                    std::string updateTotalHit, double grade) {
  if (key.length() == 0) {
    return false;
  }

  SummaryMap distinct;
  distinct["dist_key"] = key;

  if (distCount > 0) {
    distinct["dist_count"] = distCount;
  }

  if (distTimes > 0) {
    distinct["dist_times"] = distTimes;
  }

  if (reserved.length() != 0) {
    distinct["reserved"] = reserved;
  }

  if (distFilter.length() != 0) {
    distinct["dist_filter"] = distFilter;
  }

  if (updateTotalHit.length() != 0) {
    distinct["update_total_hit"] = updateTotalHit;
  }

  if (grade > 0) {
    distinct["grade"] = grade;
  }

  this->distinct_[key] = distinct;
  return true;
}

std::string CloudsearchSearch::getDistinctString() {
  StringSummaryMap &distinct = this->getDistinct();
  return ToString(distinct);
}

int CloudsearchSearch::getRerankSize() const {
  try {
    SummaryMap::const_iterator it = this->configMap_.begin();
    if (it != this->configMap_.end()) {
      return utils::AnyCast<int>(it->second);
    }
  } catch (utils::BadAnyCast &e) {
    // ignore
  }
  return 200;  //默认200
}

std::string CloudsearchSearch::getDisableFunctions() {
  std::string result = "";
  for (SummaryMap::iterator it = this->disable_.begin();
      it != this->disable_.end(); ++it) {
    result += ';' + it->first;
    if (it->second.toString().length() > 0) {
      result += ':' + it->second.toString();
    }
  }
  if (result.length() > 0) {
    return result.substr(1);
  }
  return "";
}

void CloudsearchSearch::addFilter(std::string filter, std::string op) {
  if (op.length() == 0) {
    op = "AND";
  }
  if (filter_.length() == 0) {
    filter_ = filter;
  } else {
    filter_ += ' ' + op + ' ' + filter;
  }
}

static bool isNotBlank(string str) {
  if (str.length() != 0 && utils::StringUtils::trim(str).length() != 0) {
    return true;
  }
  return false;
}

std::string CloudsearchSearch::call(SearchTypeEnum type) {
  std::map<std::string, std::string> params;

  std::string haQuery = "";
  if (type == SearchTypeEnum::SEARCH) {
    haQuery.append("config=").append(this->clauseConfig()).append("&&").append(
        "query=").append(isNotBlank(this->getQuery()) ? this->getQuery() : "''")
        .append(
        isNotBlank(this->getSortString()) ?
            "&&sort=" + this->getSortString() : "").append(
        isNotBlank(this->getFilter()) ? "&&filter=" + this->getFilter() : "")
        .append(
        isNotBlank(this->getDistinctString()) ?
            "&&distinct=" + this->getDistinctString() : "").append(
        isNotBlank(this->getAggregateString()) ?
            "&&aggregate=" + this->getAggregateString() : "").append(
        isNotBlank(this->getPair()) ? "&&kvpairs=" + this->getPair() : "");
  } else if (type == SearchTypeEnum::SCROLL) {
    haQuery.append("config=").append(this->clauseConfig()).append("&&").append(
        "query=").append(isNotBlank(this->getQuery()) ? this->getQuery() : "''")
        .append(
        isNotBlank(this->getFilter()) ? "&&filter=" + this->getFilter() : "")
        .append(
        isNotBlank(this->getPair()) ? "&&kvpairs=" + this->getPair() : "");
    if (isNotBlank(this->getScrollExpire())) {
      params["scroll"] = this->getScrollExpire();
    }
    if (isNotBlank(this->getScrollId())) {
      params["scroll_id"] = this->getScrollId();
    } else {
      params["search_type"] = SEARCH_TYPE_SCAN;
    }
  }
  params["query"] = haQuery;

  std::string searchIndexes;
  if (this->indexes_.size() > 0) {
    for (int i = 0; i < this->indexes_.size(); ++i) {
      searchIndexes += ';' + utils::StringUtils::ToString(this->indexes_[i]);
    }
  }
  if (searchIndexes.length() > 0) {
    params["index_name"] = searchIndexes.substr(1);
  } else {
    params["index_name"] = "";
  }
  params["format"] = this->getFormat();

  if (isNotBlank(this->getFormulaName())) {
    params["formula_name"] = this->getFormulaName();
  }

  if (isNotBlank(this->getFirstFormulaName())) {
    params["first_formula_name"] = this->getFirstFormulaName();
  }

  if (isNotBlank(this->getSummaryString())) {
    params["summary"] = this->getSummaryString();
  }

  if (this->fetches_.size() > 0) {
    std::string fetchFields;
    for (int i = 0; i < this->fetches_.size(); ++i) {
      fetchFields += ';' + this->fetches_[i];
    }
    params["fetch_fields"] = fetchFields.substr(1);
  }

  if (this->qp_.size() > 0) {
    std::string qpNames;
    for (int i = 0; i < this->qp_.size(); ++i) {
      qpNames += ',' + this->qp_[i];
    }
    params["qp"] = qpNames.substr(1);
  }

  if (this->getDisableFunctions().length() > 0) {
    params["disable"] = this->getDisableFunctions();
  }

  if (this->customParams_.size() > 0) {
    for (std::map<std::string, std::string>::iterator it = this->customParams_
        .begin(); it != this->customParams_.end(); ++it) {
      params[it->first] = it->second;
    }
  }
  bool isPB = "protobuf" == getFormat();
  return this->client_->call(this->path_, params, CloudsearchClient::METHOD_GET,
                             isPB, this->debugInfo_);
}

std::string CloudsearchSearch::clauseConfig() {
  return ToString(this->configMap_);
}
void CloudsearchSearch::disableQp(
    std::map<std::string, std::vector<std::string> >& opts) {
  std::string processorConfig = "";  //整个qp需要关闭的子项配置
  std::string processor = "";
  std::string indexNames = "";

  for (std::map<std::string, std::vector<std::string> >::iterator it = opts
      .begin(); it != opts.end(); ++it) {
    processor = it->first;
    indexNames = "";
    if (!it->second.empty()) {
      indexNames = ":";
      indexNames += this->getIndexInQp(it->second);
    }
    processor += indexNames;
    processorConfig += ',' + processor;
  }
  this->disable_["qp"] = processorConfig.substr(1);
}
std::string CloudsearchSearch::getIndexInQp(std::vector<std::string> indexes) {
  std::string indexNames = "";
  for (int i = 0; i < indexes.size(); ++i) {
    indexNames += '|' + indexes[i];
  }
  if (indexNames.length() > 0) {
    return indexNames.substr(1);
  }
  return "";
}
void CloudsearchSearch::clear() {
  this->aggregate_.clear();
  this->customParams_.clear();
  this->distinct_.clear();
  this->fetches_.clear();
  this->filter_ = "";
  this->firstFormulaName_ = "";
  this->formulaName_ = "";
  this->indexes_.clear();
  this->kvpair_ = "";
  this->query_ = "";
  this->sort_.clear();
  this->summary_.clear();
  this->qp_.clear();
  this->disable_.clear();
  this->scroll_ = "";
  this->scrollId_ = "";
  this->searchType_ = "";
  initCustomConfigMap();
}

void CloudsearchSearch::removeDistinct(std::string distinctKey) {
  StringSummaryMap &distinct = this->getDistinct();
  if (distinct.find(distinctKey) != distinct.end()) {
    distinct.erase(distinctKey);
  }
}

std::string CloudsearchSearch::search() {
  SummaryMap emptyMap;
  return this->search(emptyMap);
}
} /* namespace opensearch */
} /* namespace aliyun */
