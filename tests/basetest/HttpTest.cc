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

#include "aliyun/http/HttpRequest.h"
#include "aliyun/http/HttpResponse.h"
#include "aliyun/http/X509TrustAll.h"

using aliyun::Exception;
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

TEST(HTTP, GET) {
  HttpRequest request("http://acs.aliyun-inc.com/");
  request.setMethod(MethodType::GET);
  request.putHeaderParameter("Accept", "*/*");
  request.putHeaderParameter(
      "User-Agent",
      "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.80 Safari/537.36");

  HttpResponse response = doHttpRequest(request);
  EXPECT_TRUE(response.getStatus() == 400);
  EXPECT_TRUE(response.getContent().length() > 0);
}

TEST(HTTP, PUT) {
  std::string content =
      "<Product name=\"Yundun\" domain=\"yundun.aliyuncs.com\"/>";
  HttpRequest request("http://acs.aliyun-inc.com/");
  request.setMethod(MethodType::PUT);

  HttpResponse response = doHttpRequest(request);
  EXPECT_TRUE(response.getStatus() == 400);
  EXPECT_TRUE(response.getContent().length() > 0);
}

// DONE: add unit test for POST, etc
TEST(HTTP, POST) {
  // set up a web server on localhost
  HttpRequest request("http://203.88.161.174:8080/servlet/service?user=name&pass=word");
  request.setMethod(MethodType::POST);

  HttpResponse response = doHttpRequest(request);
  EXPECT_TRUE(response.getHeaderValue("Request-Method") == "POST");  // servlet
  EXPECT_TRUE(response.getContent().length() > 0);
}

TEST(HTTP, HEAD) {
  HttpRequest request("http://203.88.161.174:8080/servlet/service");
  request.setMethod(MethodType::HEAD);

  HttpResponse response = doHttpRequest(request);
  EXPECT_TRUE(response.getHeaderValue("Request-Method") == "HEAD");  // servlet
  EXPECT_TRUE(response.getContent().length() == 0);
}

TEST(HTTP, DeleteTest) {
  HttpRequest request("http://203.88.161.174:8080/servlet/service");
  request.setMethod(MethodType::Delete);

  HttpResponse response = doHttpRequest(request);
  EXPECT_TRUE(response.getHeaderValue("Request-Method") == "DELETE");  // servlet
}

TEST(HTTP, OPTIONS) {
  HttpRequest request("http://acs.aliyun-inc.com/");
  request.setMethod(MethodType::OPTIONS);

  HttpResponse response = doHttpRequest(request);
  EXPECT_TRUE(response.getContent().length() == 0);
}

TEST(HTTPS, test) {
  HttpRequest request("https://acs.aliyun-inc.com/");

  {
    X509TrustAll trustAll;  // SSL peer certificate or SSH remote key was not OK
    HttpResponse response = doHttpRequest(request);

    EXPECT_TRUE(response.getStatus() == 400);
    EXPECT_TRUE(response.getContent().length() > 0);
  }
}

void testHttpStatus(std::string url, long status) {
  HttpRequest request(url);
  HttpResponse response = doHttpRequest(request);

  EXPECT_EQ(status, response.getStatus());
}

TEST(HTTP, status) {
  testHttpStatus("http://g.cn/generate_204", 204);
}
