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

#ifndef ALIYUN_AUTH_CREDENTIAL_H_
#define ALIYUN_AUTH_CREDENTIAL_H_

#include <ctime>
#include <string>

#include "aliyun/utils/date.h"

namespace aliyun {
namespace auth {

class Credential {
 public:
  typedef utils::Date Date;
  typedef std::string string;

  Credential() {
    // TODO(xu): ensure default time zone.
    refreshDate_ = Date::currentUtcDate();
  }

  Credential(string keyId, string secret) {
    accessKeyId_ = keyId;
    accessSecret_ = secret;
    refreshDate_ = Date::currentUtcDate();
  }

  Credential(string keyId, string secret, int expiredHours) {
    accessKeyId_ = keyId;
    accessSecret_ = secret;
    refreshDate_ = Date::currentUtcDate();

    setExpiredDate(expiredHours);
  }

  bool isExpired() const {
    if (expiredDate_.invalid()) {
      return true;
    }
    if (expiredDate_ < Date::currentUtcDate()) {
      return true;
    }
    return false;
  }

  // getter and setters
  const string& getAccessKeyId() const {
    return accessKeyId_;
  }

  void setAccessKeyId(string accessKeyId) {
    this->accessKeyId_ = accessKeyId;
  }

  const string& getAccessSecret() const {
    return accessSecret_;
  }

  void setAccessSecret(string accessSecret) {
    this->accessSecret_ = accessSecret;
  }

  const Date& getRefreshDate() const {
    return refreshDate_;
  }

  void setRefreshDate(const Date& refreshDate) {
    this->refreshDate_ = refreshDate;
  }

  const string& getSecurityToken() const {
    return securityToken_;
  }

  void setSecurityToken(string securityToken) {
    this->securityToken_ = securityToken;
  }

  const Date& getExpiredDate() const {
    return expiredDate_;
  }

 private:
  void setExpiredDate(int expiredHours) {
    if (expiredHours > 0) {
      time_t refresh_time = refreshDate_.ctime();
      time_t expire_time = refresh_time + 3600;
      expiredDate_ = Date(expire_time);
    }
  }

  Date refreshDate_;
  Date expiredDate_;
  string accessKeyId_;
  string accessSecret_;
  string securityToken_;
};

}  // namespace auth
}  // namespace aliyun

#endif  // ALIYUN_AUTH_CREDENTIAL_H_
