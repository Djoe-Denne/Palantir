if(NOT GTEST_FOUND)
    return()
endif()

set(TEST_TARGET_NAME application_tests)

add_executable(${TEST_TARGET_NAME}
    # Add test source files here
    main_test.cpp
)

setup_target_testing(${TEST_TARGET_NAME})

target_link_libraries(${TEST_TARGET_NAME}
    PRIVATE
        application
)

add_test(NAME ${TEST_TARGET_NAME} COMMAND ${TEST_TARGET_NAME}) 