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

#include <stdlib.h>
#include <gtest/gtest.h>

#include "aliyun/reader/JsonReader.h"

using aliyun::Exception;
using aliyun::reader::JsonReader;
using aliyun::reader::JsonException;

class JsonReaderTest : public ::testing::Test {
 protected:
  std::string json_;
  std::map<std::string, std::string> map_;

 public:
  JsonReaderTest() {
    json_ = "{\"Instances\":{\"Instance\":[{\"CreationTime\":\"2015-01-14T03:31:26Z\",\"Description\":\"test escape \\\"Timeout\\\"\",\"EipAddress\":{\"AllocationId\":\"\",\"IpAddress\":\"127.0.0.1\",\"InternetChargeType\":\"\"},\"HostName\":\"iZ25dbbj9izZ\",\"ImageId\":\"centos7u0_64_20G_aliaegis_20141117.vhd\",\"InnerIpAddress\":{\"IpAddress\":[\"127.0.0.101\",\"127.0.0.102\"]},\"InstanceId\":\"i-25dbbj9iz\",\"InstanceName\":\"iZ25dbbj9izZ-vnc-260755\",\"InstanceNetworkType\":\"classic\",\"InstanceType\":\"ecs.t1.xsmall\",\"InternetChargeType\":\"PayByBandwidth\",\"InternetMaxBandwidthIn\":200,\"InternetMaxBandwidthOut\":200,\"OperationLocks\":{\"LockReason\":[]},\"PublicIpAddress\":{\"IpAddress\":[]},\"RegionId\":\"cn-beijing\",\"SecurityGroupIds\":{\"SecurityGroupId\":[\"sg-259bpu24m\"]},\"Status\":\"Running\",\"VlanId\":\"\",\"VpcAttributes\":{\"NatIpAddress\":\"\",\"PrivateIpAddress\":{\"IpAddress\":[]},\"VSwitchId\":\"\",\"VpcId\":\"\"},\"ZoneId\":\"cn-beijing-a\",\"ClusterId\":\"\"},{\"CreationTime\":\"2015-01-05T03:43:02Z\",\"Description\":\"\",\"EipAddress\":{\"AllocationId\":\"\",\"IpAddress\":\"\",\"InternetChargeType\":\"\"},\"HostName\":\"iZ25fk8a1ngZ\",\"ImageId\":\"centos7u0_64_20G_aliaegis_20141117.vhd\",\"InnerIpAddress\":{\"IpAddress\":[]},\"InstanceId\":\"i-25fk8a1ng\",\"InstanceName\":\"ESS-sg-vpc??????2-ecs-i-25fk8a1ng\",\"InstanceNetworkType\":\"vpc\",\"InstanceType\":\"ecs.t1.xsmall\",\"InternetChargeType\":\"\",\"InternetMaxBandwidthIn\":0,\"InternetMaxBandwidthOut\":0,\"OperationLocks\":{\"LockReason\":[]},\"PublicIpAddress\":{\"IpAddress\":[]},\"RegionId\":\"cn-beijing\",\"SecurityGroupIds\":{\"SecurityGroupId\":[\"sg-25k9r0l1s\"]},\"Status\":\"Running\",\"VlanId\":\"\",\"VpcAttributes\":{\"NatIpAddress\":\"10.175.65.9\",\"PrivateIpAddress\":{\"IpAddress\":[\"172.16.0.7\"]},\"VSwitchId\":\"vsw-25y0u4t0d\",\"VpcId\":\"vpc-25nhkqvej\"},\"ZoneId\":\"cn-beijing-a\",\"ClusterId\":\"\"}]},\"PageNumber\":1,\"PageSize\":10,\"TotalCount\":2,\"RequestId\":\"A11D2D03-8DE3-43E6-B362-625DEBF3F88F\"}";
  }

  virtual void SetUp() {
    try {
      JsonReader jsonReader;
      map_ = jsonReader.read(json_, "DescribeInstancesResponse");

      std::map<std::string, std::string>::iterator it;
      for (it = map_.begin(); it != map_.end(); it++) {
        std::cout << "[" << it->first << "] => [" << it->second << "]\n";
      }
    } catch (JsonException& e) {
      std::cout << "JsonException: " << e.what();
    } catch (Exception& e) {
      std::cout << "Exception: " << e.what();
    } catch (...) {
      std::cout << "unknow exception throwed!";
    }
  }
};

TEST_F(JsonReaderTest, numberTest) {
  EXPECT_EQ("2",
      map_["DescribeInstancesResponse.TotalCount"]);
  EXPECT_EQ("200",
      map_["DescribeInstancesResponse.Instances[0].InternetMaxBandwidthIn"]);
}

TEST_F(JsonReaderTest, structTest) {
  EXPECT_EQ("",
      map_["DescribeInstancesResponse.Instances[0].EipAddress.AllocationId"]);
  EXPECT_EQ("127.0.0.1",
      map_["DescribeInstancesResponse.Instances[0].EipAddress.IpAddress"]);
}

TEST_F(JsonReaderTest, stringTest) {
  EXPECT_EQ("A11D2D03-8DE3-43E6-B362-625DEBF3F88F",
      map_["DescribeInstancesResponse.RequestId"]);
  EXPECT_EQ("centos7u0_64_20G_aliaegis_20141117.vhd",
      map_["DescribeInstancesResponse.Instances[0].ImageId"]);
  EXPECT_EQ("test escape \"Timeout\"",
      map_["DescribeInstancesResponse.Instances[0].Description"]);
}

TEST_F(JsonReaderTest, listTest) {
  EXPECT_EQ("2",
      map_["DescribeInstancesResponse.Instances.Length"]);
  EXPECT_EQ("2",
      map_["DescribeInstancesResponse.Instances[0].InnerIpAddress.Length"]);
  EXPECT_EQ("127.0.0.101",
      map_["DescribeInstancesResponse.Instances[0].InnerIpAddress[0]"]);
  EXPECT_EQ("127.0.0.102",
      map_["DescribeInstancesResponse.Instances[0].InnerIpAddress[1]"]);
  EXPECT_EQ(map_.end(),
      map_.find(
          "DescribeInstancesResponse.Instances[0].OperationLocks.Length"));
}

// cover all escape sequence in JsonReader.
TEST(JsonReaderTest, testEscape) {
//  escapes['\\'] = '\\';
//  escapes['/'] = '/';
//  escapes['"'] = '"';
//  escapes['t'] = '\t';
//  escapes['n'] = '\n';
//  escapes['r'] = '\r';
//  escapes['b'] = '\b';
//  escapes['f'] = '\f';
  std::string json = "{\"top\":{\"escape\":{\"CR\":\"\\r\",\"LF\":\"\\n\","
      "\"TAB\":\"\\t\",\"BACK\":\"\\b\",\"FEED\":\"\\f\",\"QUOTE\":\"\\\"\","
      "\"SLASH\":\"\\/\",\"BSLASH\":\"\\\\\"}}}";
  std::cout << "json>" << json << std::endl;

  aliyun::reader::JsonReader reader;
  std::map<std::string, std::string> result = reader.read(json, "prefix");

  EXPECT_EQ("\r", result["prefix.top.escape.CR"]);
  EXPECT_EQ("\n", result["prefix.top.escape.LF"]);
  EXPECT_EQ("\t", result["prefix.top.escape.TAB"]);
  EXPECT_EQ("\b", result["prefix.top.escape.BACK"]);
  EXPECT_EQ("\f", result["prefix.top.escape.FEED"]);
  EXPECT_EQ("\"", result["prefix.top.escape.QUOTE"]);
  EXPECT_EQ("/", result["prefix.top.escape.SLASH"]);
  EXPECT_EQ("\\", result["prefix.top.escape.BSLASH"]);
}

std::map<std::string, std::string> safeParse(std::string content,
                                             std::string prefix) {
  std::map<std::string, std::string> result;
  aliyun::reader::JsonReader reader;

  std::cout << "parse(" << content << ", " << prefix << "): ";
  try {
    result = reader.read(content, prefix);
    std::cout << "well.\n";
    for (std::map<std::string, std::string>::iterator it = result.begin();
        it != result.end(); ++it) {
      std::cout << "[" << it->first << "] => [" << it->second << "]\n";
    }
  } catch (aliyun::Exception& e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "unknow exception!\n" << std::endl;
  }
  return result;
}

TEST(JsonReaderTest, testBroken) {
  safeParse("{", "prefix");
  safeParse("{[\"key\"}", "prefix");
  safeParse("{\"name\": \"value}", "prefix");
  safeParse("{\"format\": \"json\", \"status\": \"well-formed\"}", "test");
  safeParse("{,,}", "more-comma");
  safeParse("{\"name\"::\"value\"}", "more-colon");
}
