include_directories(${GTEST_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

set(TEST_LIBRARIES gtest gtest_main)
set(SDK_LIBRARIES aliyun-opensearch curl apr-1 aprutil-1)

set(TEST_SOURCE_FILES
        opensearch/object/SingleDocTest.cc
        opensearch/object/DocItemsTest.cc
        opensearch/object/TypesTest.cc
)

add_executable(UnitTestes ${TEST_SOURCE_FILES})
target_link_libraries(UnitTestes ${SDK_LIBRARIES} ${TEST_LIBRARIES})
