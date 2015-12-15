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

#ifndef ALIYUN_READER_READERFACTORY_H_
#define ALIYUN_READER_READERFACTORY_H_

#include <stddef.h>

#include "../http/FormatType.h"
#include "JsonReader.h"
#include "XmlReader.h"

namespace aliyun {
namespace reader {

class ReaderFactory {
 public:
  typedef aliyun::http::FormatType FormatType;

  static Reader* createInstance(FormatType format) {
    if (FormatType::JSON == format) {
      return new JsonReader();
    }
    if (FormatType::XML == format) {
      return new XmlReader();
    }
    return NULL;
  }
};


}  // namespace reader
}  // namespace aliyun

#endif  // ALIYUN_READER_READERFACTORY_H_
