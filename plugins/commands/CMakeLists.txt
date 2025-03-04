set(MODULE_NAME "commands-plugin")

# Collect source files
set(COMMANDS_PLUGIN_SOURCES
    ${PROJECT_ROOT}/plugins/commands/src/command/commands_plugin.cpp
    ${PROJECT_ROOT}/plugins/commands/src/command/show_command.cpp
    ${PROJECT_ROOT}/plugins/commands/src/command/stop_command.cpp
    ${PROJECT_ROOT}/plugins/commands/src/command/window_screenshot_command.cpp
    ${PROJECT_ROOT}/plugins/commands/src/command/toggle_transparency_command.cpp
    ${PROJECT_ROOT}/plugins/commands/src/command/toggle_window_anonymity_command.cpp
    ${PROJECT_ROOT}/plugins/commands/src/command/clear_screenshot_command.cpp
    ${PROJECT_ROOT}/plugins/commands/src/command/send_sauron_request_command.cpp
)

set(COMMANDS_PLUGIN_HEADERS
    ${PROJECT_ROOT}/plugins/commands/include/command/commands_plugin.hpp
    ${PROJECT_ROOT}/plugins/commands/include/command/show_command.hpp
    ${PROJECT_ROOT}/plugins/commands/include/command/stop_command.hpp
    ${PROJECT_ROOT}/plugins/commands/include/command/window_screenshot_command.hpp
    ${PROJECT_ROOT}/plugins/commands/include/command/toggle_transparency_command.hpp
    ${PROJECT_ROOT}/plugins/commands/include/command/toggle_window_anonymity_command.hpp
    ${PROJECT_ROOT}/plugins/commands/include/command/clear_screenshot_command.hpp
    ${PROJECT_ROOT}/plugins/commands/include/command/send_sauron_request_command.hpp
    ${PROJECT_ROOT}/plugins/commands/include/plugin_export.hpp
)

if(WIN32)
    set(COMMANDS_PLUGIN_SOURCES
        ${COMMANDS_PLUGIN_SOURCES}
        ${PROJECT_ROOT}/plugins/commands/src/platform/windows/command/window_screenshot_command.cpp
        ${PROJECT_ROOT}/plugins/commands/src/platform/windows/command/toggle_transparency_command.cpp
    )
elseif(APPLE)
    set(COMMANDS_PLUGIN_SOURCES
        ${COMMANDS_PLUGIN_SOURCES}
        ${PROJECT_ROOT}/plugins/commands/src/platform/macos/command/window_screenshot_command.cpp
        ${PROJECT_ROOT}/plugins/commands/src/platform/macos/command/toggle_transparency_command.cpp
    )
endif()

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

# Add export definition
target_compile_definitions(${MODULE_NAME} PRIVATE COMMANDS_PLUGIN_EXPORTS)

# Platform specific setup
if(WIN32)
    target_compile_definitions(${MODULE_NAME} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

# Set output directories - all outputs go to predictable locations
set_target_properties(${MODULE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Add a post-build command to copy the plugin to the plugins directory
add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/plugins"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${MODULE_NAME}> "${CMAKE_BINARY_DIR}/bin/plugins/"
)

# Link against palantir-core
target_link_libraries(${MODULE_NAME} PRIVATE palantir-core)

add_subdirectory(tests) 