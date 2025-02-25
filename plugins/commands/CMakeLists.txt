set(MODULE_NAME "commands-plugin")

# Collect source files
set(COMMANDS_PLUGIN_SOURCES
    ${PROJECT_ROOT}/plugins/commands/src/command/commands_plugin.cpp
    ${PROJECT_ROOT}/plugins/commands/src/command/show_command.cpp
    ${PROJECT_ROOT}/plugins/commands/src/command/stop_command.cpp
)

set(COMMANDS_PLUGIN_HEADERS
    ${PROJECT_ROOT}/plugins/commands/include/command/commands_plugin.hpp
    ${PROJECT_ROOT}/plugins/commands/include/command/show_command.hpp
    ${PROJECT_ROOT}/plugins/commands/include/command/stop_command.hpp
    ${PROJECT_ROOT}/plugins/commands/include/utils/auto_command_register.hpp
)

set(ALL_SOURCES
    ${COMMANDS_PLUGIN_SOURCES}
    ${COMMANDS_PLUGIN_HEADERS}
)

# Create shared library
add_library(${MODULE_NAME} SHARED
    ${COMMANDS_PLUGIN_SOURCES}
    ${COMMANDS_PLUGIN_HEADERS}
)

target_include_directories(${MODULE_NAME} 
    PRIVATE 
        ${PROJECT_ROOT}/plugins/commands/include
        ${PROJECT_ROOT}/palantir-core/include
        ${COMMON_INCLUDE_DIRS}
)

# Platform specific setup
if(WIN32)
    target_compile_definitions(${MODULE_NAME} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

# Set output directories
set_target_properties(${MODULE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
)

# Link against palantir-core
target_link_libraries(${MODULE_NAME} PRIVATE palantir-core) 