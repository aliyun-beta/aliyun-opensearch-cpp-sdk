include_directories(${GTEST_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

set(TEST_LIBRARIES gtest gtest_main)
set(SDK_LIBRARIES aliyun-opensearch curl apr-1 aprutil-1)

set(TEST_SOURCE_FILES
        opensearch/object/SingleDocTest.cc
        opensearch/object/DocItemsTest.cc
        opensearch/object/TypesTest.cc
        opensearch/object/SchemaTableTest.cc
        opensearch/CloudsearchClientTest.cc
        opensearch/CloudsearchSearchTest.cc
)

add_executable(UnitTestes ${TEST_SOURCE_FILES})
target_link_libraries(UnitTestes ${SDK_LIBRARIES} ${TEST_LIBRARIES})

set(BASE_TEST_FILES
        basetest/Base64Test.cc
        basetest/CredentialTest.cc
        basetest/HmacTest.cc
        basetest/HttpTypesTest.cc
        basetest/ParamterHelperTest.cc
        )

add_executable(BaseTestes ${BASE_TEST_FILES})
target_link_libraries(BaseTestes ${SDK_LIBRARIES} ${TEST_LIBRARIES})