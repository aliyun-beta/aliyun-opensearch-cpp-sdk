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

#include "aliyun/reader/XmlReader.h"
#include "aliyun/utils/StringUtils.h"
#include "aliyun/utils/details/GlobalInitializer.h"

namespace aliyun {

namespace reader {

XmlReader::XmlReader()
    : pool_(NULL),
      pdoc_(NULL),
      parser_(NULL) {
  prepareParser();
}

XmlReader::~XmlReader() {
  if (pool_) {
    apr_pool_destroy(pool_);
  }
}

apr_xml_doc* XmlReader::getDocument(string xml) {
  apr_status_t rc;
  char emsg[256];
#define apr_check_throw(rc) \
        if (APR_SUCCESS != rc) throw XmlException(apr_strerror(rc, emsg, sizeof(emsg)));

  // feed input to parser
  rc = apr_xml_parser_feed(parser_, xml.c_str(), xml.length());
  apr_check_throw(rc);

  rc = apr_xml_parser_done(parser_, &pdoc_);
  apr_check_throw(rc);

  return pdoc_;
}

std::string XmlReader::getContent(node* n) {
  string text;
  for (apr_text* t = n->first_cdata.first; t; t = t->next) {
    text += t->text;
  }
  return text;
}

const char* XmlReader::getAttribute(node* n, string name) {
  for (apr_xml_attr* a = n->attr; a; a = a->next) {
    if (a->name == name) {
      return a->value;
    }
  }
  return NULL;
}

std::vector<XmlReader::node*> XmlReader::getElementsByTagName(document* doc,
                                                              string tag) {
  return getChildElements(doc->root, tag);;
}

std::vector<XmlReader::node*> XmlReader::getElementsByTagName(node* curr,
                                                              string tag) {
  std::vector<node*> result;
  for (node* n = curr->first_child; n; n = n->next) {
    if (n->name == tag) {
      result.push_back(n);
    } else {
      std::vector<node*> more = getElementsByTagName(n, tag);
      result.insert(result.end(), more.begin(), more.end());
    }
  }
  return result;
}

std::map<std::string, std::string> XmlReader::read(string response,
                                                   string endpoint) {
  getDocument(response.c_str());

  apr_xml_elem* root = pdoc_->root;

  read(root, endpoint.c_str(), false);

  return map_;
}

void XmlReader::dump() {
  if (NULL == pdoc_)
    return;
  dumpElements(pdoc_->root, 0);
}

std::vector<apr_xml_elem*> XmlReader::getChildElements(apr_xml_elem* parent) {
  std::vector<apr_xml_elem*> childs;
  for (apr_xml_elem* c = parent->first_child; c; c = c->next) {
    childs.push_back(c);
  }
  return childs;
}

std::vector<apr_xml_elem*> XmlReader::getChildElements(apr_xml_elem* parent,
                                                       string tagName) {
  std::vector<apr_xml_elem*> childs;
  for (apr_xml_elem* c = parent->first_child; c; c = c->next) {
    if (c->name == tagName) {
      childs.push_back(c);
    }
  }
  return childs;
}

void XmlReader::dumpElements(apr_xml_elem* e, int depth) {
  if (e == NULL)
    return;

  std::string prepends(depth * 4, ' ');
  ::printf("%s", prepends.c_str());
  ::printf("#%s\n", e->name);

  for (apr_xml_attr* a = e->attr; a; a = a->next) {
    ::printf("%s", prepends.c_str());
    ::printf(" {%s: %s}\n", a->name, a->value);
  }

  for (apr_text* p = e->first_cdata.first; p; p = p->next) {
    ::printf("%s   ", prepends.c_str());
    ::printf("%s\n", p->text);
  }

  for (apr_text* p = e->following_cdata.first; p; p = p->next) {
    ::printf("%s  $", prepends.c_str());
    ::printf("%s\n", p->text);
  }

  for (apr_xml_elem* p = e->first_child; p; p = p->next) {
    dumpElements(p, depth + 1);
  }
}

void XmlReader::prepareParser() {
  apr_status_t rc;
  char emsg[256];

  if ((rc = apr_pool_create(&pool_, NULL)) != APR_SUCCESS) {
    throw XmlException(apr_strerror(rc, emsg, sizeof(emsg)));
  }

  parser_ = apr_xml_parser_create(pool_);
  if (parser_ == NULL) {
    throw XmlException(apr_strerror(rc, emsg, sizeof(emsg)));
  }
}

void XmlReader::read(apr_xml_elem* element, string path, bool appendPath) {
  path = buildPath(element, path, appendPath);
  std::vector<apr_xml_elem*> children = getChildElements(element);

  // leaf node
  if (children.size() == 0) {
    map_[path] = getContent(element);
    return;
  }

  // otherwise, internal node

  // find all children with the same name as children[0]
  std::vector<apr_xml_elem*> sameName = getChildElements(element,
                                                         children[0]->name);

  if (sameName.size() > 1 && children.size() == sameName.size()) {
    // more than one child has same name as children[0]
    // likes, <parent><child>xx</child>...<child>yy</child></parent>
    elementsAsList(children, path);
  } else if (sameName.size() == 1 && children.size() == 1) {
    // only one child under current element
    // likes, <parent><child>...</child></parent>
    elementsAsList(children, path);
    read(children[0], path, true);
  } else {
    for (size_t i = 0; i < children.size(); i++) {
      read(children[i], path, true);
    }
  }
}

std::string XmlReader::buildPath(apr_xml_elem* element, string& path,
                                 bool appendPath) {
  return appendPath ? path + "." + element->name : path;
}

void XmlReader::elementsAsList(std::vector<apr_xml_elem*>& elems, string path) {
  using namespace aliyun::utils::StringUtils;
  map_[path + ".Length"] = ToString(elems.size());
  for (size_t i = 0; i < elems.size(); i++) {
    read(elems[i], path + "[" + ToString(i) + "]", false);
  }
}

}  // namespace reader

}  // namespace aliyun
