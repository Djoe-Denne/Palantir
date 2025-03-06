set(TEST_TARGET_NAME palantir_core_tests)

add_executable(${TEST_TARGET_NAME}
    # Add test source files here
        main_test.cpp
    command/command_factory_test.cpp
    input/key_config_test.cpp
    input/key_mapper_test.cpp
    input/key_register_test.cpp
    input/configurable_input_test.cpp
    input/input_factory_test.cpp
    signal/signal_factory_test.cpp
    signal/signal_test.cpp
    window/window_manager_test.cpp
    utils/string_utils_test.cpp
)

message(STATUS "Setting up testing for target ${TEST_TARGET_NAME}")
setup_target_testing(${TEST_TARGET_NAME})

# Add dependencies to ensure libraries are built first
add_dependencies(${TEST_TARGET_NAME} palantir-core)

target_link_libraries(${TEST_TARGET_NAME}
    PRIVATE
        palantir-core
)

target_include_directories(${TEST_TARGET_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/palantir-core/tests
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