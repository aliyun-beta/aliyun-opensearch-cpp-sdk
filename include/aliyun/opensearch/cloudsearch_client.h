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

#ifndef ALIYUN_OPENSEARCH_CLOUDSEARCH_CLIENT_H_
#define ALIYUN_OPENSEARCH_CLOUDSEARCH_CLIENT_H_

#include <time.h>
#include <map>
#include <string>

#include "../auth/url_encoder.h"
#include "../auth/hmac_sha1.h"
#include "../http/http_request.h"
#include "../http/http_response.h"
#include "../utils/date.h"
#include "../utils/parameter_helper.h"
#include "../utils/string_utils.h"
#include "object/key_type_enum.h"

namespace aliyun {
class Exception;
}  // namespace aliyun

namespace aliyun {
namespace opensearch {

class CloudsearchClient {
 public:
  typedef std::string string;
  typedef string& stringref;
  typedef object::KeyTypeEnum KeyTypeEnum;

  /**
   * 指定默认的请求方式；默认为GET.
   */
  static const string DEFAULT_METHOD;  // = "GET";

  static const string METHOD_GET;  // = "GET";

  static const string METHOD_POST;  // = "POST";

  /**
   * 构造函数
   *
   * @param clientId 用户的client id，从网站中可以获得此信息。
   * @param clientSecret 用户的client secret，从网站中可以获得此信息。
   * @param opts 一些可选信息，包含：
   *              version 当前使用的API版本，默认值为 ：v2
   *              host 指定请求的host地址
   *              timeout 指定请求超时时间，单位为：微秒。用户可以根据自己的场景来设定此值， 例如如果搜索可以设定时间稍短，如果推送文档，可以设定稍长的时间。默认为10000
   *              connect_timeout 指定连接超时时间，单位为：微秒。默认为5000
   *              gzip 指定使用gzip方式传输数据，默认为false
   * @throws UnknownHostException
   * @donotgenetatedoc
   */
  CloudsearchClient(string clientId, string clientSecret, string host,
                    const std::map<string, string>& opts);

  /**
   * 构造函数
   *
   * @param accesskey 用户的accesskey，从网站中可以获得此信息。
   * @param secret 用户的 secret，从网站中可以获得此信息。
   * @param opts 一些可选信息，包含：
   *            version 当前使用的API版本，默认值为v2。
   *            host 指定请求的host地址，默认为：http://opensearch-cn-hangzhou.aliyuncs.com
   *            timeout 指定请求超时时间，单位为：微秒。用户可以根据自己的场景来设定此值，例如如果搜索可以设定时间稍短，如果推送文档，可以设定稍长的时间。单位为：微妙，默认值为10000
   *            connect_timeout 指定连接超时时间，单位为：微秒，默认值为5000
   * @param keyType 指定当前的用户类型，取值范围为：KeyTypeEnum.OPENSEARCH，KeyTypeEnum.ALIYUN。默认值为KeyTypeEnum.OPENSEARCH
   * @throws UnknownHostException
   */
  CloudsearchClient(string accesskey, string secret, string host,
                    const std::map<string, string>& opts, KeyTypeEnum keyType);

  void setMaxConnections(int maxConns);

  /**
   * 向服务器发出请求并获得返回结果
   *
   * @param path 当前请求的path路径。
   * @param params 当前请求的所有参数数组。
   * @param method 当前请求的方法，取值为CloudsearchClient.METHOD_GET或者CloudsearchClient.METHOD_POST。默认值为CloudsearchClient.METHOD_GET
   * @return string 返回获取的结果。
   */
  string call(string path, const std::map<string, string>& params,
              string method, bool isPB) throw(aliyun::Exception) {
    string debugInfo;
    return call(path, params, method, isPB, debugInfo);
  }

  /**
   * 向服务器发出请求并获得返回结果
   *
   * @param path 当前请求的path路径。
   * @param params 当前请求的所有参数数组。
   * @param isPB 是否为protobuf类型，默认为false
   * @return 返回获取的结果。
   * @throws IOException
   * @throws ClientProtocolException
   * @donotgenetatedoc
   */
  string call(string path, const std::map<string, string>& params, bool isPB)
                  throw(aliyun::Exception) {
    return call(path, params, DEFAULT_METHOD, isPB);
  }

  /**
   * 向服务器发出请求并获得返回结果
   *
   * @param path 当前请求的path路径。
   * @param params 当前请求的所有参数数组。
   * @param method 当前请求的方法，取值为CloudsearchClient.METHOD_GET或者CloudsearchClient.METHOD_POST。默认值为：CloudsearchClient.METHOD_GET
   * @param isPB 是否为protobuf类型，默认为false
   * @param debugInfo 当前请求的调试信息
   * @return string 返回获取的结果。
   * @throws IOException
   */
  string call(string path, const std::map<string, string>& params,
              string method, bool isPB, stringref debugInfo);
  /**
   * 向服务器发出请求并获得返回结果
   *
   * @param path 当前请求的path路径。
   * @param params 当前请求的所有参数数组。
   * @param method 当前请求的方法，取值为CloudsearchClient.METHOD_GET或者CloudsearchClient.METHOD_POST。默认值为：CloudsearchClient.METHOD_GET
   * @return 搜索结果
   * @throws ClientProtocolException
   * @throws IOException
   * @donotgenetatedoc
   */
  string call(string path, const std::map<string, string>& params,
              string method) throw(aliyun::Exception) {
    return call(path, params, method, false);
  }

  /**
   * 向服务器发出请求并获得返回结果
   *
   * @param path 当前请求的path路径。
   * @param params 当前请求的所有参数数组。
   * @param method 当前请求的方法，取值为CloudsearchClient.METHOD_GET或者CloudsearchClient.METHOD_POST。默认值为：CloudsearchClient.METHOD_GET
   * @param debugInfo 当前请求的调试信息
   * @return string 返回获取的结果。
   */
  string call(string path, const std::map<string, string>& params,
              string method, stringref debugInfo) throw(aliyun::Exception) {
    return call(path, params, method, false, debugInfo);
  }

 private:
  string getNonce();

  string buildQuery(const std::map<string, string>& params);

  string doSign(std::map<string, string>* params);

  static string buildHttpParameterString(const std::map<string, string>& param);

  string getAliyunSign(std::map<string, string>* params, string method);

  string doRequest(string url, const std::map<string, string>& requestParams,
                   string method, bool isPB);

  /**
   * 用户的client id。
   *
   * 此信息由网站中提供。
   */
  string clientId_;

  /**
   * 用户的秘钥。
   *
   * 此信息由网站中提供。
   */
  string clientSecret_;

  /**
   * 请求API的base URI.
   */
  string baseURI_;

  /**
   * 当前API的版本号。
   */
  string version_;  // = "v2";

  /**
   * 请求的domain地址。
   */
  string host_;

  /**
   * 用户类型，包含opensearch老用户和阿里云用户
   */
  KeyTypeEnum keyType_;  // = KeyTypeEnum::OPENSEARCH;

  /**
   * 用户阿里云网站中的accesskey,keyYype为ALIYUN使用 此信息阿里云网站中提供
   */
  string accesskey_;

  /**
   * 用户阿里云网站中的secret,keyYype为ALIYUN使用 此信息阿里云网站中提供
   */
  string secret_;

  void initialize(const string &clientId, const string &clientSecret,
                  const string &host, const std::map<string, string> &opts);
};

}  // namespace opensearch
}  // namespace aliyun

#endif  // ALIYUN_OPENSEARCH_CLOUDSEARCH_CLIENT_H_
