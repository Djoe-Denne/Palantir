set(MODULE_NAME "plugin-loader")

# Collect source files
set(PLUGIN_LOADER_SOURCES
    ${PROJECT_ROOT}/plugin-loader/src/plugin_loader.cpp
    ${PROJECT_ROOT}/plugin-loader/src/plugin_manager.cpp
)

set(PLUGIN_LOADER_HEADERS
    ${PROJECT_ROOT}/plugin-loader/include/plugin_loader/iplugin.hpp
    ${PROJECT_ROOT}/plugin-loader/include/plugin_loader/plugin_loader.hpp
    ${PROJECT_ROOT}/plugin-loader/include/plugin_loader/plugin_manager.hpp
)

# Create static library
add_library(${MODULE_NAME} STATIC
    ${PLUGIN_LOADER_SOURCES}
    ${PLUGIN_LOADER_HEADERS}
)

target_include_directories(${MODULE_NAME} 
    PUBLIC 
        ${PROJECT_ROOT}/plugin-loader/include
    PRIVATE
        ${COMMON_INCLUDE_DIRS}
)

# Platform specific setup
if(WIN32)
    target_compile_definitions(${MODULE_NAME} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

# Set output directories
set_target_properties(${MODULE_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Link against palantir-core as we'll need some of its functionality
target_link_libraries(${MODULE_NAME} PUBLIC palantir-core) 