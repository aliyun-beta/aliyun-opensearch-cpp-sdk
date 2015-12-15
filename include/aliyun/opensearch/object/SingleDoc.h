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

#ifndef ALIYUN_OPENSEARCH_OBJECT_SINGLEDOC_H_
#define ALIYUN_OPENSEARCH_OBJECT_SINGLEDOC_H_

#include <map>
#include <string>

namespace aliyun {
namespace opensearch {
namespace object {

class SingleDoc {
 public:
  typedef std::string string;

  SingleDoc();

  SingleDoc(string cmd, const std::map<string, string>& fields);

  void addField(string key, string value);

  const string& getCommand() const {
    return command_;
  }

  void setCommand(string command) {
    command_ = command;
  }

  const std::map<string, string>& getFields() const {
    return fields_;
  }

  string getJsonString() const;

 private:
  string command_;
  std::map<string, string> fields_;
};

}  // namespace object
}  // namespace opensearch
}  // namespace aliyun

#endif  // ALIYUN_OPENSEARCH_OBJECT_SINGLEDOC_H_
