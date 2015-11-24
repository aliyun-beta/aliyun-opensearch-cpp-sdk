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
#include "aliyun/opensearch/object/SingleDoc.h"

using aliyun::opensearch::SingleDoc;

TEST(SingleDocTest, ctor) {
  SingleDoc doc1;
  EXPECT_EQ("", doc1.getCommand());
  EXPECT_EQ("", doc1.getJsonString());

  std::map<std::string, std::string> fields;
  SingleDoc doc2("", fields);
  EXPECT_EQ("", doc2.getCommand());
  EXPECT_EQ("", doc2.getJsonString());
}

TEST(SingleDocTest, json) {
  SingleDoc doc1;
  EXPECT_EQ("", doc1.getCommand());
  EXPECT_EQ("", doc1.getJsonString());

  std::map<std::string, std::string> fields;

  SingleDoc doc2("doc2", fields);
  EXPECT_EQ("doc2", doc2.getCommand());
  EXPECT_EQ("{cmd:doc2}", doc2.getJsonString());

  fields["foo"] = "bar";
  fields["k1"] = "v1";
  fields["k2"] = "v2";
  fields["k3"] = "v3";

  SingleDoc doc3("doc3", fields);
  EXPECT_EQ("doc3", doc3.getCommand());
  EXPECT_EQ("{cmd:doc3,{foo:bar,k1:v1,k2:v2,k3:v3}}", doc3.getJsonString());
}

TEST(SingleDocTest, addField) {
  SingleDoc doc;
  doc.setCommand("doc");
  doc.addField("keywords", "food\035sweat");
  doc.addField("filter", "200g\035express");
  EXPECT_EQ("{cmd:doc,{filter:[200g,express],keywords:[food,sweat]}}", doc.getJsonString());
}