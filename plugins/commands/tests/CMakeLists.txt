if(NOT GTEST_FOUND)
    return()
endif()

set(TEST_TARGET_NAME commands_plugin_tests)

add_executable(${TEST_TARGET_NAME}
    main_test.cpp
    command/commands_plugin_test.cpp
    command/show_command_test.cpp
    command/stop_command_test.cpp
    command/window_screenshot_command_test.cpp
    command/windows_screenshot_command_test.cpp
    command/macos_screenshot_command_test.cpp
    command/toggle_transparency_command_test.cpp
    command/toggle_window_anonymity_command_test.cpp
    command/clear_screenshot_command_test.cpp
)

setup_target_testing(${TEST_TARGET_NAME})

# Add dependencies to ensure libraries are built first
add_dependencies(${TEST_TARGET_NAME} commands-plugin palantir-core)

target_link_libraries(${TEST_TARGET_NAME}
    PRIVATE
        commands-plugin
        palantir-core
)

target_include_directories(${TEST_TARGET_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/plugins/commands/include
        ${CMAKE_SOURCE_DIR}/plugins/commands/tests
        ${CMAKE_SOURCE_DIR}/palantir-core/include
        ${GTEST_INCLUDE_DIRS}
        ${GMOCK_INCLUDE_DIRS}
)

# Let CMake know where to search for the library
link_directories(${CMAKE_BINARY_DIR}/lib)

# Set output directories for the test executable
set_target_properties(${TEST_TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
    PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Register test with CTest
add_test(
    NAME ${TEST_TARGET_NAME}
    COMMAND ${TEST_TARGET_NAME}
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Set test properties
set_tests_properties(${TEST_TARGET_NAME} PROPERTIES
    ENVIRONMENT "PATH=${CMAKE_BINARY_DIR}/bin;$ENV{PATH}"
) 