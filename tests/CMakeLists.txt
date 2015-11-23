add_executable(SingleDocTest opensearch/object/SingleDocTest.cc)
target_link_libraries(SingleDocTest aliyun-opensearch curl apr-1 aprutil-1 gtest gtest_main)

if(AOSS_BUILD_TEST)
    if (WIN32 AND (NOT CYGWIN) AND (NOT MINGW))
        set(gtest_disable_pthreads ON)
        set(gtest_force_shared_crt ON)
    endif()
    include_directories(${GTEST_INCLUDE_DIR})

    set(TEST_LIBRARIES gtest gtest_main)


    target_link_libraries(SingleDocTest ${TEST_LIBRARIES})
endif()