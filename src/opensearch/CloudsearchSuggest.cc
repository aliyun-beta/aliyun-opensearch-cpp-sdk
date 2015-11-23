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

#include "aliyun/opensearch/CloudsearchSuggest.h"

namespace aliyun {
namespace opensearch {

using std::string;

CloudsearchSuggest::CloudsearchSuggest(string indexName, string suggestName,
                                       CloudsearchClient* client) {
  client_ = client;
  indexName_ = indexName;
  suggestName_ = suggestName;
  hit_ = 10;
  path_ = "/suggest";
}

std::string CloudsearchSuggest::search() {
  std::map<std::string, std::string> params;
  params["index_name"] = this->indexName_;
  params["suggest_name"] = this->suggestName_;
  params["hit"] = utils::StringUtils::ToString(this->hit_);
  params["query"] = this->query_;

  return this->client_->call(this->path_, params, CloudsearchClient::METHOD_GET, this->debugInfo_);
}
std::string CloudsearchSuggest::getIndexName() {
  return this->indexName_;
}
std::string CloudsearchSuggest::getSuggestName() {
  return this->suggestName_;
}
void CloudsearchSuggest::setHit(int hit) {
  if (hit < 0) {
    hit = 10;
  }
  this->hit_ = hit;
}
int CloudsearchSuggest::getHit() {
  return this->hit_;
}
void CloudsearchSuggest::setQuery(std::string query) {
  this->query_ = query;
}
std::string CloudsearchSuggest::getQuery() {
  return this->query_;
}
std::string CloudsearchSuggest::getDebugInfo() {
  return this->debugInfo_;
}
} /* namespace opensearch */
} /* namespace aliyun */
