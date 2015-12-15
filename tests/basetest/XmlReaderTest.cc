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
#include "aliyun/reader/XmlReader.h"

using aliyun::Exception;
using aliyun::reader::XmlReader;
using aliyun::reader::XmlException;

#define ARRAYSIZE(a) sizeof(a)/sizeof(a[0])

class XmlReaderTest : public testing::Test {
 protected:
  std::string xml_;
  std::map<std::string, std::string> map_;

 public:
  XmlReaderTest() {
    xml_ = "<?xml version=\"1.0\" encoding=\"utf-8\"?><DescribeInstancesResponse><Instances><Instance><CreationTime>2014-10-13T11:51:22Z</CreationTime><Description></Description><EipAddress><AllocationId></AllocationId><IpAddress>127.0.0.1</IpAddress><InternetChargeType></InternetChargeType></EipAddress><HostName>iZ23a8l47rmZ</HostName><ImageId>centos6u5_64_20G_aliaegis_20140926.vhd</ImageId><InnerIpAddress><IpAddress>10.132.60.90</IpAddress><IpAddress>10.132.60.91</IpAddress></InnerIpAddress><InstanceId>i-23a8l47rm</InstanceId><InstanceName>iZ23a8l47r&amp;mZ-vncpwd-869735</InstanceName><InstanceNetworkType>classic</InstanceNetworkType><InstanceType>ecs.t1.xsmall</InstanceType><InternetChargeType></InternetChargeType><InternetMaxBandwidthIn>-1</InternetMaxBandwidthIn><InternetMaxBandwidthOut>1</InternetMaxBandwidthOut><OperationLocks></OperationLocks><PublicIpAddress><IpAddress>121.199.43.126</IpAddress></PublicIpAddress><RegionId>cn-hangzhou</RegionId><SecurityGroupIds><SecurityGroupId>G1344371</SecurityGroupId></SecurityGroupIds><Status>Stopped</Status><VlanId></VlanId><VpcAttributes><NatIpAddress></NatIpAddress><PrivateIpAddress></PrivateIpAddress><VSwitchId></VSwitchId><VpcId></VpcId></VpcAttributes><ZoneId>cn-hangzhou-b</ZoneId><ClusterId></ClusterId></Instance></Instances><PageNumber>1</PageNumber><PageSize>10</PageSize><TotalCount>1</TotalCount><RequestId>555FF875-0341-4D16-BFD1-E5B96FFEACC5</RequestId></DescribeInstancesResponse>";
  }

  virtual void SetUp() {
    try {
      XmlReader reader;
      map_ = reader.read(xml_, "DescribeInstancesResponse");
    } catch (...) {
      std::cout << "unknow exception!\n" << std::endl;
    }

    for (std::map<std::string, std::string>::iterator it = map_.begin();
         it != map_.end(); ++it) {
      std::cout << "\"" << it->first << "\" => \"" << it->second << "\"\n";
    }
  }
};

TEST_F(XmlReaderTest, numberTest) {
  EXPECT_EQ("1", map_["DescribeInstancesResponse.TotalCount"]);
  EXPECT_EQ("-1",
      map_["DescribeInstancesResponse.Instances[0].InternetMaxBandwidthIn"]);
}

TEST_F(XmlReaderTest, structTest) {
  EXPECT_EQ("",
      map_["DescribeInstancesResponse.Instances[0].EipAddress.AllocationId"]);
  EXPECT_EQ("127.0.0.1",
      map_["DescribeInstancesResponse.Instances[0].EipAddress.IpAddress"]);
}

TEST_F(XmlReaderTest, stringTest) {
  EXPECT_EQ("555FF875-0341-4D16-BFD1-E5B96FFEACC5",
      map_["DescribeInstancesResponse.RequestId"]);
  EXPECT_EQ("centos6u5_64_20G_aliaegis_20140926.vhd",
      map_["DescribeInstancesResponse.Instances[0].ImageId"]);
}

TEST_F(XmlReaderTest, listTest) {
  EXPECT_EQ("1",
      map_["DescribeInstancesResponse.Instances.Length"]);
  EXPECT_EQ("2",
      map_["DescribeInstancesResponse.Instances[0].InnerIpAddress.Length"]);
  EXPECT_EQ("10.132.60.90",
      map_["DescribeInstancesResponse.Instances[0].InnerIpAddress[0]"]);
  EXPECT_EQ("10.132.60.91",
      map_["DescribeInstancesResponse.Instances[0].InnerIpAddress[1]"]);
  EXPECT_EQ("1",
      map_["DescribeInstancesResponse.Instances[0].PublicIpAddress.Length"]);
  EXPECT_EQ(map_.end(),
      map_.find(
          "DescribeInstancesResponse.Instances[0].OperationLocks.Length"));
}

void testXml(std::string xml) {
  try {
    aliyun::reader::XmlReader reader;
    std::map<std::string, std::string> map = reader.read(xml, "prefix");
    reader.dump();
    for (std::map<std::string, std::string>::iterator it = map.begin();
        it != map.end(); ++it) {
      std::cout << "\"" << it->first << "\" => \"" << it->second << "\"\n";
    }
  } catch (std::exception& e) {
    std::cout << "exception: " << e.what() << std::endl;
  } catch (...) {
    std::cout << "unknow exception!\n" << std::endl;
  }
}

// for side cases
TEST(XmlReaderTest, sideCases) {
  std::string sideCases[] = {
      // only one child
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
          "<root>"
          "<child>No.1</child>"
          "</root>",

      // more than one child has same name as 1st child
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
          "<root>"
          "<child>No.1</child>"
          "<child>No.2</child>"
          "</root>",

      // normal case
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
          "<root>"
          "<name>test name</name>"
          "<value>test value</value>"
          "</root>",

      // normal case, with attribute
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
          "<root>"
          "<config name=\"nnnn\" value=\"vvvv\">"
          "test contents"
          "</config>"
          "</root>",

       // mixed case
       "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
          "<root>"
          "<name>first name</name>"
          "<name>second name</name>"  // corrupt
          "<value>phone number</value>"
          "</root>" };
  for (int i = 0; i < ARRAYSIZE(sideCases); i++) {
    testXml(sideCases[i]);
  }
}
