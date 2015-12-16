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

#include <gtest/gtest.h>
#include "aliyun/opensearch/object/doc_items.h"

using aliyun::opensearch::object::DocItems;
using aliyun::opensearch::object::SingleDoc;

TEST(DocItemsTest, add_json) {
  std::map<std::string, std::string> fields;
  fields["foo"] = "bar";
  SingleDoc doc1("doc1", fields);

  fields["have"] = "fun";
  SingleDoc doc2("doc2", fields);

  DocItems docItems;
  docItems.addDoc(doc1);
  docItems.addDoc(doc2);

  EXPECT_EQ(
      "{{\"cmd\":\"doc1\",\"fields\":{\"foo\":\"bar\"}},{\"cmd\":\"doc2\",\"fields\":{\"foo\":\"bar\",\"have\":\"fun\"}}}",
      docItems.getJsonString());
}
