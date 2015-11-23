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

#ifndef CORE_READER_XMLREADER_H_
#define CORE_READER_XMLREADER_H_

#include <apr_xml.h>
#include <map>
#include <string>
#include <vector>

#include "../Exception.h"
#include "Reader.h"

namespace aliyun {

namespace reader {

class XmlException : public aliyun::Exception {
 public:
  XmlException(std::string msg)
      : aliyun::Exception(msg) {
  }
};

class XmlReader : public Reader {

 public:

  typedef std::string string;
  typedef apr_xml_doc document;
  typedef apr_xml_elem node;

  XmlReader();

  ~XmlReader();

  apr_xml_doc * getDocument(string xml);

  static string getContent(node* n);

  static const char * getAttribute(node* n, string name);

  static std::vector<node*> getElementsByTagName(document* doc, string tag);

  static std::vector<node*> getElementsByTagName(node* curr, string tag);

  std::map<string, string> read(string response, string endpoint);

  void dump();

  static std::vector<apr_xml_elem*> getChildElements(apr_xml_elem* parent);

  static std::vector<apr_xml_elem*> getChildElements(apr_xml_elem* parent,
                                                     string tagName);

 private:

  static void dumpElements(apr_xml_elem* e, int depth);

  void prepareParser();

  void read(apr_xml_elem* element, string path, bool appendPath);

  string buildPath(apr_xml_elem* element, string& path, bool appendPath);

  void elementsAsList(std::vector<apr_xml_elem*>& elems, string path);

  std::map<string, string> map_;
  apr_pool_t* pool_;
  apr_xml_doc* pdoc_;
  apr_xml_parser* parser_;
};

}  // namespace reader

}  // namespace aliyun

#endif  // CORE_READER_XMLREADER_H_
