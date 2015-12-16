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
        file(GLOB_RECURSE APU_LIB_PATHS ${PROJECT_SOURCE_DIR}/thirdparty/apr-util-1.5.4/*aprutil*.lib)
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
            file(GLOB_RECURSE APU_LIB_PATHS ${PROJECT_SOURCE_DIR}/thirdparty/apr-util-1.5.4/*aprutil*.lib)
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
        include/aliyun/opensearch.h
        include/aliyun/auth/url_encoder.h
        include/aliyun/auth/credential.h
        include/aliyun/auth/hmac_sha1.h
        include/aliyun/auth/hmac_sha256.h
        include/aliyun/auth/icredential_provider.h
        include/aliyun/auth/isigner.h
        include/aliyun/exception.h
        include/aliyun/http/format_type.h
        include/aliyun/http/http_request.h
        include/aliyun/http/http_response.h
        include/aliyun/http/method_type.h
        include/aliyun/http/protocol_type.h
        include/aliyun/http/x509_trust_all.h
        include/aliyun/opensearch/cloudsearch_client.h
        include/aliyun/opensearch/cloudsearch_doc.h
        include/aliyun/opensearch/cloudsearch_index.h
        include/aliyun/opensearch/cloudsearch_search.h
        include/aliyun/opensearch/cloudsearch_suggest.h
        include/aliyun/opensearch/object/doc_items.h
        include/aliyun/opensearch/object/key_type_enum.h
        include/aliyun/opensearch/object/schema_table_field.h
        include/aliyun/opensearch/object/schema_table_field_type.h
        include/aliyun/opensearch/object/schema_table.h
        include/aliyun/opensearch/object/search_type_enum.h
        include/aliyun/opensearch/object/single_doc.h
        include/aliyun/reader/json_reader.h
        include/aliyun/reader/reader_factory.h
        include/aliyun/reader/reader.h
        include/aliyun/reader/xml_reader.h
        include/aliyun/utils/any.h
        include/aliyun/utils/base64_helper.h
        include/aliyun/utils/date.h
        include/aliyun/utils/parameter_helper.h
        include/aliyun/utils/string_utils.h
        include/aliyun/utils/details/global_initializer.h
        )

set(SOURCE_FILES
        ${HEADER_FILES}
        src/auth/url_encoder.cc
        src/auth/hmac_sha1.cc
        src/auth/hmac_sha256.cc
        src/http/format_type.cc
        src/http/http_request.cc
        src/http/http_response.cc
        src/http/method_type.cc
        src/http/protocol_type.cc
        src/opensearch/cloudsearch_client.cc
        src/opensearch/cloudsearch_doc.cc
        src/opensearch/cloudsearch_index.cc
        src/opensearch/cloudsearch_search.cc
        src/opensearch/cloudsearch_suggest.cc
        src/opensearch/object/doc_items.cc
        src/opensearch/object/key_type_enum.cc
        src/opensearch/object/schema_table.cc
        src/opensearch/object/schema_table_field.cc
        src/opensearch/object/schema_table_field_type.cc
        src/opensearch/object/search_type_enum.cc
        src/opensearch/object/single_doc.cc
        src/reader/json_reader.cc
        src/reader/xml_reader.cc
        src/utils/base64_helper.cc
        src/utils/date.cc
        src/utils/parameter_helper.cc
        src/utils/string_utils.cc
        src/utils/details/global_initializer.cc
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
