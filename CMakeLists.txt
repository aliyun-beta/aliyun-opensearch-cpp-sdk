cmake_minimum_required(VERSION 3.3)
project(aopensearch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        core/auth/AcsURLEncoder.h
        core/auth/AcsURLEncoder.cc
        core/auth/Credential.h
        core/auth/HmacSha1.h
        core/auth/HmacSha1.cc
        core/auth/HmacSha256.h
        core/auth/HmacSha256.cc
        core/auth/ICredentialProvider.h
        core/auth/ISignatureComposer.h
        core/auth/ISignatureComposer.cc
        core/auth/ISigner.h
        core/auth/OssSignatureComposer.h
        core/auth/OssSignatureComposer.cc
        core/auth/RoaSignatureComposer.h
        core/auth/RoaSignatureComposer.cc
        core/auth/RpcSignatureComposer.h
        core/auth/RpcSignatureComposer.cc
        core/http/FormatType.h
        core/http/FormatType.cc
        core/http/HttpRequest.cc
        core/http/HttpRequest.h
        core/http/HttpResponse.cc
        core/http/HttpResponse.h
        core/http/MethodType.h
        core/http/MethodType.cc
        core/http/ProtocolType.h
        core/http/ProtocolType.cc
        core/http/X509TrustAll.h
        core/reader/JsonReader.h
        core/reader/JsonReader.cc
        core/reader/Reader.h
        core/reader/ReaderFactory.h
        core/reader/XmlReader.h
        core/reader/XmlReader.cc
        core/utils/Any.h
        core/utils/Base64Helper.cc
        core/utils/Base64Helper.h
        core/utils/Date.cc
        core/utils/Date.h
        core/utils/ParameterHelper.h
        core/utils/ParameterHelper.cc
        core/utils/StringUtils.cc
        core/utils/StringUtils.h
        core/Exception.h
        opensearch/object/SingleDoc.cc
        opensearch/object/SingleDoc.h
        opensearch/object/DocItems.cc
        opensearch/object/DocItems.h
        opensearch/object/KeyTypeEnum.cc
        opensearch/object/KeyTypeEnum.h
        opensearch/object/SchemaTable.cc
        opensearch/object/SchemaTable.h
        opensearch/object/SchemaTableField.cc
        opensearch/object/SchemaTableField.h
        opensearch/object/SchemaTableFieldType.cc
        opensearch/object/SchemaTableFieldType.h
        opensearch/object/SearchTypeEnum.cc
        opensearch/object/SearchTypeEnum.h
        opensearch/CloudsearchClient.cc
        opensearch/CloudsearchClient.h
        opensearch/CloudsearchDoc.cc
        opensearch/CloudsearchDoc.h
        opensearch/CloudsearchIndex.cc
        opensearch/CloudsearchIndex.h
        opensearch/CloudsearchSearch.cc
        opensearch/CloudsearchSearch.h
        opensearch/CloudsearchSuggest.cc
        opensearch/CloudsearchSuggest.h
)


set(APR_INCLUDES /usr/include/apr-1.0/)

include_directories(${APR_INCLUDES})

add_library(aliyun-opensearch ${SOURCE_FILES})
