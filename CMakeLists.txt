cmake_minimum_required(VERSION 2.8)
project(AliyunOpenSearchSDK CXX)

SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_BUILD_TYPE RelWithDebInfo)

option(AOSS_BUILD_TEST "Build Aliyun OpenSeach SDK unittests" ON)
option(PREFER_SYSTEM_LIB "Prefer system library to build" ON)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -march=native -Wall -Wextra")
else () # if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
endif ()


# check curl
if (PREFER_SYSTEM_LIB)
    find_package(CURL)
endif ()

if (CURL_FOUND)
    message(STATUS "use system curl library!")
else ()
    find_path(CURL_INCLUDE_DIR curl/curl.h PATHS ${PROJECT_SOURCE_DIR}/thirdparty/curl-7.45.0/include NO_DEFAULT_PATH)
    if (UNIX)
        find_library(CURL_LIBRARY curl PATHS ${PROJECT_SOURCE_DIR}/thirdparty/curl-7.45.0/lib/.libs NO_DEFAULT_PATH)
    else ()
        file(GLOB_RECURSE CURL_LIB_PATHS ${PROJECT_SOURCE_DIR}/thirdparty/curl-7.45.0/*.lib)
        list(GET CURL_LIB_PATHS 0 CURL_LIBRARY)
    endif ()
    message(CURL_LIBRARY: ${CURL_LIBRARY})
    if (NOT CURL_LIBRARY)
        if (UNIX)
            execute_process(COMMAND ./unix_build.sh curl WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/thirdparty/)
        else ()
            execute_process(COMMAND win_build.bat curl WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/thirdparty/)
        endif ()
        if (UNIX)
            find_library(CURL_LIBRARY curl PATHS ${PROJECT_SOURCE_DIR}/thirdparty/curl-7.45.0/lib/.libs NO_DEFAULT_PATH)
        else ()
            file(GLOB_RECURSE CURL_LIB_PATHS ${PROJECT_SOURCE_DIR}/thirdparty/curl-7.45.0/*.lib)
            list(GET CURL_LIB_PATHS 0 CURL_LIBRARY)
        endif ()
    endif ()
endif ()
set(AOSS_INCLUDES ${AOSS_INCLUDES} ${CURL_INCLUDE_DIR})


# check apr
if (PREFER_SYSTEM_LIB AND UNIX)
    find_path(APR_INCLUDE_DIR apr.h)
    find_library(APR_LIBRARY apr-1)
    if (NOT APR_INCLUDE_DIR)
        file(GLOB_RECURSE APR_H_DIRS "/usr/*apr.h")
        list(GET APR_H_DIRS 0 APR_INCLUDE_DIR)
        string(REPLACE "apr.h" "" APR_INCLUDE_DIR ${APR_INCLUDE_DIR})
    endif ()
endif ()

if (APR_INCLUDE_DIR AND APR_LIBRARY)
    message(STATUS "use system apr library!")
else ()
    find_path(APR_INCLUDE_DIR apr.h PATHS ${PROJECT_SOURCE_DIR}/thirdparty/apr-1.5.2/include NO_DEFAULT_PATH)
    if (UNIX)
        find_library(APR_LIBRARY apr-1 PATHS ${PROJECT_SOURCE_DIR}/thirdparty/apr-1.5.2/.libs NO_DEFAULT_PATH)
    else ()
        file(GLOB_RECURSE APR_LIB_PATHS ${PROJECT_SOURCE_DIR}/thirdparty/apr-1.5.2/*.lib)
        message(test: ${APR_LIB_PATHS})
        list(GET APR_LIB_PATHS 0 APR_LIBRARY)
    endif ()
    if (NOT APR_LIBRARY)
        if (UNIX)
            execute_process(COMMAND ./unix_build.sh apr WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/thirdparty/)
        else ()
            execute_process(COMMAND win_build.bat apr WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/thirdparty/)
        endif ()
        find_path(APR_INCLUDE_DIR apr.h PATHS ${PROJECT_SOURCE_DIR}/thirdparty/apr-1.5.2/include NO_DEFAULT_PATH)
        if (UNIX)
            find_library(APR_LIBRARY apr-1 PATHS ${PROJECT_SOURCE_DIR}/thirdparty/apr-1.5.2/.libs NO_DEFAULT_PATH)
        else ()
            file(GLOB_RECURSE APR_LIB_PATHS ${PROJECT_SOURCE_DIR}/thirdparty/apr-1.5.2/*.lib)
            list(GET APR_LIB_PATHS 0 APR_LIBRARY)
        endif ()
    endif ()
endif ()
set(AOSS_INCLUDES ${AOSS_INCLUDES} ${APR_INCLUDE_DIR})


# check apr-util
if (PREFER_SYSTEM_LIB AND UNIX)
    find_path(APU_INCLUDE_DIR apu.h)
    find_library(APU_LIBRARY aprutil-1)
    if (NOT APU_INCLUDE_DIR)
        file(GLOB_RECURSE APU_H_DIRS "/usr/*apu.h")
        list(GET APU_H_DIRS 0 APU_INCLUDE_DIR)
        string(REPLACE "apu.h" "" APU_INCLUDE_DIR ${APU_INCLUDE_DIR})
    endif ()
endif ()

if (APU_INCLUDE_DIR AND APU_LIBRARY)
    message(STATUS "use system apr-util library!")
else ()
    find_path(APU_INCLUDE_DIR apu.h PATHS ${PROJECT_SOURCE_DIR}/thirdparty/apr-util-1.5.4/include NO_DEFAULT_PATH)
    message(apu_include_dir: ${APU_INCLUDE_DIR})
    if (UNIX)
        find_library(APU_LIBRARY aprutil-1 PATHS ${PROJECT_SOURCE_DIR}/thirdparty/apr-util-1.5.4/.libs NO_DEFAULT_PATH)
    else ()
        file(GLOB_RECURSE APU_LIB_PATHS ${PROJECT_SOURCE_DIR}/thirdparty/apr-util-1.5.4/*.lib)
        list(GET APU_LIB_PATHS 0 APU_LIBRARY)
    endif ()
    if (NOT APU_LIBRARY)
        if (UNIX)
            execute_process(COMMAND ./unix_build.sh apu WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/thirdparty/)
        else ()
            execute_process(COMMAND win_build.bat apu WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/thirdparty/)
        endif ()
        find_path(APU_INCLUDE_DIR apu.h PATHS ${PROJECT_SOURCE_DIR}/thirdparty/apr-util-1.5.4/include NO_DEFAULT_PATH)
        if (UNIX)
            find_library(APU_LIBRARY aprutil-1 PATHS ${PROJECT_SOURCE_DIR}/thirdparty/apr-util-1.5.4/.libs NO_DEFAULT_PATH)
        else ()
            file(GLOB_RECURSE APU_LIB_PATHS ${PROJECT_SOURCE_DIR}/thirdparty/apr-util-1.5.4/*.lib)
            list(GET APU_LIB_PATHS 0 APU_LIBRARY)
        endif ()
    endif ()
endif ()
set(AOSS_INCLUDES ${AOSS_INCLUDES} ${APU_INCLUDE_DIR})

message(CURL_INCLUDE_DIR: ${CURL_INCLUDE_DIR})
message(CURL_LIBRARY: ${CURL_LIBRARY})
message(APR_INCLUDE_DIR: ${APR_INCLUDE_DIR})
message(APR_LIBRARY: ${APR_LIBRARY})
message(APU_INCLUDE_DIR: ${APU_INCLUDE_DIR})
message(APU_LIBRARY: ${APU_LIBRARY})

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
