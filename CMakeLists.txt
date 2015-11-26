cmake_minimum_required(VERSION 2.8)
project(AliyunOpenSearchSDK CXX)

option(AOSS_BUILD_TEST "Build Aliyun OpenSeach SDK unittests" ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -march=native -Wall -Wextra -rdynamic")
    set(AOSS_INCLUDES ${AOSS_INCLUDES} /usr/include/apr-1.0/)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -march=native -Wall -Wextra")
    #set(AOSS_INCLUDES ${AOSS_INCLUDES} /* apr include path */)
    #set(AOSS_INCLUDES ${AOSS_INCLUDES} /* curl include path */)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
    #set(AOSS_INCLUDES ${AOSS_INCLUDES} /* apr include path */)
    #set(AOSS_INCLUDES ${AOSS_INCLUDES} /* curl include path */)
endif ()

# TODO: check dependencies: curl, apr, apr-util
find_package(CURL)
if (CURL_FOUND)
    message(STATUS "curl already installed!")
else ()
    message(STATUS "no curl installed!")
    find_path(CURL_INCLUDE_DIR curl.h HINTS thirdparty/curl-7.45.0)
    find_library(CURL_LIBRARY curl HINTS thirdparty/curl-7.45.0)
    if ((NOT CURL_INCLUDE_DIR) OR (NOT CURL_LIBRARY))
        if (UNIX)
            execute_process(COMMAND thirdparty/unix_build.sh curl)
        else ()
            execute_process(COMMAND thirdparty/win_build.bat curl)
        endif ()
    endif ()
endif ()

find_path(APR_INCLUDE_DIR apr.h)
find_library(APR_LIBRARY apr-1)
if (APR_INCLUDE_DIR OR APR_LIBRARY)
    message(STATUS "apr already installed!")
else ()
    message(STATUS "no apr installed!")
    find_path(APR_INCLUDE_DIR apr.h HINTS thirdparty/apr-1.5.2/include)
    find_library(APR_LIBRARY apr-1 HINTS thirdparty/apr-1.5.2)
    if ((NOT APR_INCLUDE_DIR) OR (NOT APR_LIBRARY))
        if (UNIX)
            execute_process(COMMAND thirdparty/unix_build.sh apr)
        else ()
            execute_process(COMMAND thirdparty/win_build.bat apr)
        endif ()
    endif ()
endif ()

find_path(APU_INCLUDE_DIR apu.h)
find_library(APU_LIBRARY aprutil-1)
if (APU_INCLUDE_DIR OR APU_LIBRARY)
    message(STATUS "apr-util already installed!")
else ()
    message(STATUS "no apr-util installed!")
    find_path(APU_INCLUDE_DIR apu.h HINTS thirdparty/apr-util-1.5.4/include)
    find_library(APU_LIBRARY aprutil-1 HINTS thirdparty/apr-util-1.5.4)
    if ((NOT APU_INCLUDE_DIR) OR (NOT APU_LIBRARY))
        if (UNIX)
            execute_process(COMMAND thirdparty/unix_build.sh apu)
        else ()
            execute_process(COMMAND thirdparty/win_build.bat apu)
        endif ()
    endif ()
endif ()

set(HEADER_FILES
        include/aliyun/auth/AcsURLEncoder.h
        include/aliyun/auth/Credential.h
        include/aliyun/auth/HmacSha1.h
        include/aliyun/auth/HmacSha256.h
        include/aliyun/auth/ICredentialProvider.h
        include/aliyun/auth/ISignatureComposer.h
        include/aliyun/auth/ISigner.h
        include/aliyun/auth/OssSignatureComposer.h
        include/aliyun/auth/RoaSignatureComposer.h
        include/aliyun/auth/RpcSignatureComposer.h
        include/aliyun/Exception.h
        include/aliyun/http/FormatType.h
        include/aliyun/http/HttpRequest.h
        include/aliyun/http/HttpResponse.h
        include/aliyun/http/MethodType.h
        include/aliyun/http/ProtocolType.h
        include/aliyun/http/X509TrustAll.h
        include/aliyun/opensearch/CloudsearchClient.h
        include/aliyun/opensearch/CloudsearchDoc.h
        include/aliyun/opensearch/CloudsearchIndex.h
        include/aliyun/opensearch/CloudsearchSearch.h
        include/aliyun/opensearch/CloudsearchSuggest.h
        include/aliyun/opensearch/object/DocItems.h
        include/aliyun/opensearch/object/KeyTypeEnum.h
        include/aliyun/opensearch/object/SchemaTableField.h
        include/aliyun/opensearch/object/SchemaTableFieldType.h
        include/aliyun/opensearch/object/SchemaTable.h
        include/aliyun/opensearch/object/SearchTypeEnum.h
        include/aliyun/opensearch/object/SingleDoc.h
        include/aliyun/reader/JsonReader.h
        include/aliyun/reader/ReaderFactory.h
        include/aliyun/reader/Reader.h
        include/aliyun/reader/XmlReader.h
        include/aliyun/utils/Any.h
        include/aliyun/utils/Base64Helper.h
        include/aliyun/utils/Date.h
        include/aliyun/utils/ParameterHelper.h
        include/aliyun/utils/StringUtils.h
        include/aliyun/utils/details/GlobalInitializer.h
        )

set(SOURCE_FILES
        ${HEADER_FILES}
        src/auth/AcsURLEncoder.cc
        src/auth/HmacSha1.cc
        src/auth/HmacSha256.cc
        src/auth/ISignatureComposer.cc
        src/auth/OssSignatureComposer.cc
        src/auth/RoaSignatureComposer.cc
        src/auth/RpcSignatureComposer.cc
        src/http/FormatType.cc
        src/http/HttpRequest.cc
        src/http/HttpResponse.cc
        src/http/MethodType.cc
        src/http/ProtocolType.cc
        src/opensearch/CloudsearchClient.cc
        src/opensearch/CloudsearchDoc.cc
        src/opensearch/CloudsearchIndex.cc
        src/opensearch/CloudsearchSearch.cc
        src/opensearch/CloudsearchSuggest.cc
        src/opensearch/object/DocItems.cc
        src/opensearch/object/KeyTypeEnum.cc
        src/opensearch/object/SchemaTable.cc
        src/opensearch/object/SchemaTableField.cc
        src/opensearch/object/SchemaTableFieldType.cc
        src/opensearch/object/SearchTypeEnum.cc
        src/opensearch/object/SingleDoc.cc
        src/reader/JsonReader.cc
        src/reader/XmlReader.cc
        src/utils/Base64Helper.cc
        src/utils/Date.cc
        src/utils/ParameterHelper.cc
        src/utils/StringUtils.cc
        src/utils/details/GlobalInitializer.cc
        )

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${AOSS_INCLUDES})

add_library(aliyun-opensearch ${SOURCE_FILES})

if (AOSS_BUILD_TEST)
    # build googletest firstly
    if (WIN32 AND (NOT CYGWIN) AND (NOT MINGW))
        set(gtest_disable_pthreads ON)
        set(gtest_force_shared_crt ON)
    endif ()
    set(GTEST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/thirdparty/googletest)
    add_subdirectory(${GTEST_SOURCE_DIR})

    add_subdirectory(tests)
endif ()
