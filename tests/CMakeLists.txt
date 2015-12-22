include_directories(${GTEST_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

set(TEST_LIBRARIES gtest gtest_main)
set(SDK_LIBRARIES aliyun-opensearch ${CURL_LIBRARY} ${APR_LIBRARY} ${APU_LIBRARY})

if (PCRE_LIBRARY)
    set(SDK_LIBRARIES ${SDK_LIBRARIES} ${PCRE_LIBRARY})
endif()

message(CURL_LIBRARY: ${CURL_LIBRARY})
message(APR_LIBRARY: ${APR_LIBRARY})
message(APU_LIBRARY: ${APU_LIBRARY})


set(BASE_TEST_FILES
        basetest/any_test.cc
        basetest/base64_test.cc
        basetest/credential_test.cc
        basetest/hmac_test.cc
        basetest/http_test.cc
        basetest/http_types_test.cc
        basetest/paramter_helper_test.cc
        basetest/json_reader_test.cc
        basetest/xml_reader_test.cc
        )

set(UNIT_TEST_FILES
        ${BASE_TEST_FILES}
        opensearch/object/single_doc_test.cc
        opensearch/object/doc_items_test.cc
        opensearch/object/types_test.cc
        opensearch/object/schema_table_test.cc
        opensearch/object/key_type_enum_test.cc
        opensearch/object/search_type_enum_test.cc
        opensearch/object/schema_table_field_test.cc
        opensearch/cloudsearch_client_test.cc
        opensearch/cloudsearch_search_test.cc
        opensearch/cloudsearch_doc_test.cc
        opensearch/cloudsearch_index_test.cc
        opensearch/cloudsearch_suggest_test.cc
        )

add_executable(unittests ${UNIT_TEST_FILES})
target_link_libraries(unittests ${SDK_LIBRARIES} ${TEST_LIBRARIES})

add_executable(basetests ${BASE_TEST_FILES})
target_link_libraries(basetests ${SDK_LIBRARIES} ${TEST_LIBRARIES})
