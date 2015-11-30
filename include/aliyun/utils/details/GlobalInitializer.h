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
#ifndef ALIYUN_DETAILS_GLOBALINITIALIZER_H
#define ALIYUN_DETAILS_GLOBALINITIALIZER_H

// NOTE: include this header(.h) to source(.cc/.cpp) if needed.
//       DO NOT USE FLOW CLASSES DIRECTLY!

namespace aliyun {
namespace utils {
namespace details {

class GlobalInitializer {
 public:
  static int initialize();
};

// for each include line in source(.cc) file,
// it will create a copy of follow integer when program running.
// but GlobalInitializer::initialize() is singleton like, so it works well.
static int __globalInitializer__ = GlobalInitializer::initialize();

}  // namespace details
}  // namespace utils
}  // namespace aliyun
#endif  // ALIYUN_DETAILS_GLOBALINITIALIZER_H
