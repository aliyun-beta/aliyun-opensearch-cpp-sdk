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

#include "aliyun/utils/Base64Helper.h"

namespace aliyun {

namespace utils {

const char Base64Helper::BASE64_CODE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789" "+/";

const int Base64Helper::BASE64_DECODE[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -2, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

std::string Base64Helper::encode(const byte* buffer, int length) {
  int padCount = (3 - (length % 3)) % 3;
  std::vector<byte> paddedBuffer;
  for (int i = 0; i < length + padCount; i++) {
    paddedBuffer.push_back(i < length ? buffer[i] : 0);
  }

  std::string str;
  for (int i = 0; i < (length + padCount); i += 3) {
    unsigned n = ((paddedBuffer[i] & 0xff) << 16)
        + ((paddedBuffer[i + 1] & 0xff) << 8) + (paddedBuffer[i + 2] & 0xff);
    str.push_back(BASE64_CODE[(n >> 18) & 0x3f]);
    str.push_back(BASE64_CODE[(n >> 12) & 0x3f]);
    str.push_back(BASE64_CODE[(n >> 6) & 0x3f]);
    str.push_back(BASE64_CODE[n & 0x3f]);
  }

  int len = str.length();
  for (int i = padCount; i > 0; i--) {
    str[len - i] = '=';
  }

  return str;
}

std::string Base64Helper::encode(std::string str, std::string encoding) {
  if (str.length() == 0 || encoding.length() == 0) {
    return "";
  }
  std::string dstEncStr = StringUtils::ToEncoding(str, encoding);
  const byte* bptr = reinterpret_cast<const byte*>(&dstEncStr[0]);
  return encode(bptr, dstEncStr.size());
}

std::string Base64Helper::decode(std::string str, std::string encoding) {
  if (str.length() == 0 || encoding.length() == 0) {
    return "";
  }

  int decodeLen = str.length();
  if ('=' == str[decodeLen - 1])
    decodeLen--;
  if ('=' == str[decodeLen - 1])
    decodeLen--;

  std::vector<byte> buffer(decodeLen * 3 / 4);

  int index = 0;
  int count4 = decodeLen - decodeLen % 4;

  const byte* bytes = reinterpret_cast<const byte*>(str.c_str());
  for (int i = 0; i < count4; i += 4) {
    unsigned c0 = BASE64_DECODE[bytes[i]];
    unsigned c1 = BASE64_DECODE[bytes[i + 1]];
    unsigned c2 = BASE64_DECODE[bytes[i + 2]];
    unsigned c3 = BASE64_DECODE[bytes[i + 3]];
    buffer[index++] = (byte) (((c0 << 2) | (c1 >> 4)) & 0xFF);
    buffer[index++] = (byte) ((((c1 & 0x0F) << 4) | (c2 >> 2)) & 0xFF);
    buffer[index++] = (byte) ((((c2 & 3) << 6) | c3) & 0xFF);
  }

  if (2 <= decodeLen % 4) {
    int c0 = BASE64_DECODE[bytes[count4]];
    int c1 = BASE64_DECODE[bytes[count4 + 1]];
    buffer[index++] = (byte) (((c0 << 2) | (c1 >> 4)) & 0xFF);
    if (3 == decodeLen % 4) {
      int c2 = BASE64_DECODE[bytes[count4 + 2]];
      buffer[index++] = (byte) ((((c1 & 0x0F) << 4) | (c2 >> 2)) & 0xFF);
    }
  }
  return std::string(buffer.begin(), buffer.end());
}

std::string Base64Helper::encode(std::vector<byte>& buffer) {
  return encode(&buffer[0], buffer.size());
}

}  // namespace utils

}  // namespace aliyun
