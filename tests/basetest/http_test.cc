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

#include "aliyun/http/http_request.h"
#include "aliyun/http/http_response.h"
#include "aliyun/http/x509_trust_all.h"

using aliyun::Exception;
using aliyun::http::CurlException;
using aliyun::http::MethodType;
using aliyun::http::FormatType;
using aliyun::http::HttpRequest;
using aliyun::http::HttpResponse;
using aliyun::http::X509TrustAll;

void printHttpResponse(HttpResponse response) {
#if 0
  std::cout << "RESPONSE STATUS:" << response.getStatus() << std::endl;
  std::cout << "RESPONSE HEADER:" << std::endl;
  for (std::map<std::string, std::string>::const_iterator it = response
      .getHeaders().begin(); it != response.getHeaders().end(); ++it) {
    std::cout << "\"" << it->first << "\" => \"" << it->second << "\"\n";
  }
  std::cout << "RESPONSE BODY:\n" << response.getContent() << std::endl;
#endif
}

HttpResponse doHttpRequest(HttpRequest request) {
  HttpResponse response;
  try {
    response = HttpResponse::getResponse(request);
    printHttpResponse(response);
  } catch (Exception& e) {
    std::cout << "\nException: " << e.what() << std::endl;
    std::cout << e.stackTrace() << std::endl;
  } catch (...) {
    std::cout << "unknow exceptions" << std::endl;
  }
  return response;
}

TEST(HttpTest, testGET) {
  HttpRequest request("http://acs.aliyun-inc.com/");
  request.setMethod(MethodType::GET);
  request.putHeaderParameter("Accept", "*/*");
  request.putHeaderParameter(
      "User-Agent",
      "Mozilla/5.0 (X11; Linux x86_64)");

  HttpResponse response = doHttpRequest(request);
  EXPECT_EQ(400, response.getStatus());
  EXPECT_GT(response.getContent().length(), 0);
}

TEST(HttpTest, testPUT) {
  std::string content =
      "<Product name=\"Yundun\" domain=\"yundun.aliyuncs.com\"/>";
  HttpRequest request("http://acs.aliyun-inc.com/");
  request.setMethod(MethodType::PUT);

  HttpResponse response = doHttpRequest(request);
  EXPECT_EQ(400, response.getStatus());
  EXPECT_GT(response.getContent().length(), 0);
}

// DONE: add unit test for POST, etc
TEST(HttpTest, testPOST) {
  // set up a web server on localhost
  HttpRequest request("http://aliyun.com");
  request.setMethod(MethodType::POST);

  HttpResponse response = doHttpRequest(request);
  EXPECT_TRUE(response.getContent().length() > 0);
}

TEST(HttpTest, testHEAD) {
  HttpRequest request("http://aliyun.com");
  request.setMethod(MethodType::HEAD);

  HttpResponse response = doHttpRequest(request);
  EXPECT_EQ("Tengine", response.getHeaderValue("Server"));
  EXPECT_EQ(0, response.getContent().length());
}

TEST(HttpTest, testDELETE) {
  HttpRequest request("http://aliyun.com");
  request.setMethod(MethodType::Delete);

  HttpResponse response = doHttpRequest(request);
  EXPECT_TRUE(response.getContent().length() > 0);
}

TEST(HttpTest, testOPTIONS) {
  HttpRequest request("http://acs.aliyun-inc.com/");
  request.setMethod(MethodType::OPTIONS);

  HttpResponse response = doHttpRequest(request);
  EXPECT_EQ(0, response.getContent().length());
}

TEST(HttpsTest, test) {
  {
    HttpRequest request("https://acs.aliyun-inc.com/");
    X509TrustAll trustAll;  // scoped trust
    HttpResponse response = doHttpRequest(request);

    EXPECT_EQ(0, HttpRequest::getSslVerifyHost());
    EXPECT_EQ(0, HttpRequest::getSslVerifyPeer());
    EXPECT_EQ(400, response.getStatus());
    EXPECT_GT(response.getContent().length(), 0);
  }
  EXPECT_TRUE(HttpRequest::DEFALT_VERIFYHOST_OPT == HttpRequest::getSslVerifyHost());
  EXPECT_TRUE(HttpRequest::DEFALT_VERIFYPEER_OPT == HttpRequest::getSslVerifyPeer());
}

void testHttpStatus(std::string url, int status) {
  HttpRequest request(url);
  HttpResponse response = doHttpRequest(request);

  EXPECT_EQ(status, response.getStatus());
}

TEST(HttpTest, testSTATUS) {
  testHttpStatus("http://g.cn/generate_204", 204);
}

TEST(HttpTest, testCurlException) {
  try {
    HttpRequest request("invalid URL");
    doHttpRequest(request);
  } catch (std::exception& e) {
    EXPECT_EQ(typeid(CurlException), typeid(e));
  }
}

TEST(CurlHandleTest, testCtor) {
  using aliyun::http::CurlHandle;
  CurlHandle curl;
}

TEST(CurlExceptionTest, testCtor) {
  std::string msg = "curl-exception";
  CurlException e(msg);
  EXPECT_EQ(msg, e.what());
}

TEST(HttpRequestTest, testCtor) {
  HttpRequest request1;  // default ctor
  HttpRequest request2("url");  // default ctor
  EXPECT_EQ("url", request2.getUrl());

  std::map<std::string, std::string> headers;
  headers["key1"] = "value1";
  headers["key2"] = "value2";
  HttpRequest request3("url", headers);
  EXPECT_EQ("url", request3.getUrl());
  EXPECT_EQ(2, request3.getHeaders().size());
}

TEST(HttpRequestTest, testHeaderParameter) {
  HttpRequest request;
  request.putHeaderParameter("key1", "value1");
  request.putHeaderParameter("key2", "value2");
  request.removeHeaderParameter("key1");
  EXPECT_EQ(1, request.getHeaders().size());
}

TEST(HttpRequestTest, testSetContent) {
  HttpRequest request;
  request.setContent("", "", FormatType::INVALID);
  EXPECT_EQ(0, request.getContent().length());

  std::string test_content = "test content";
  request.setContent(test_content, "", FormatType::XML);
  EXPECT_EQ(test_content, request.getContent());
}
