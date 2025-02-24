set(MODULE_NAME "palantir-core")

# Create the shared library with basic sources
add_library(${MODULE_NAME} SHARED
    ${CORE_SOURCES}
    ${COMMAND_SOURCES}
    ${SIGNAL_SOURCES}
    ${WINDOW_SOURCES}
    ${INPUT_SOURCES}
    ${APPLICATION_SOURCES}
    ${PROJECT_ROOT}/palantir-core/include/Application.hpp
    ${PROJECT_ROOT}/palantir-core/include/window/window_manager.hpp
    ${PROJECT_ROOT}/palantir-core/include/window/component/webview/iwebwiew.hpp
    ${PROJECT_ROOT}/palantir-core/include/command/icommand.hpp
    ${PROJECT_ROOT}/palantir-core/include/command/command_factory.hpp
    ${PROJECT_ROOT}/palantir-core/include/mode/debug/utils/logger.hpp
    ${PROJECT_ROOT}/palantir-core/include/input/key_register.hpp
)

target_include_directories(${MODULE_NAME} PUBLIC ${COMMON_INCLUDE_DIRS})

target_compile_definitions(${MODULE_NAME} PUBLIC PALANTIR_CORE_EXPORTS)

# Set output directories for the library
set_target_properties(${MODULE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Setup platform-specific configuration
if(WIN32)
    setup_windows_platform_core()
elseif(APPLE)
    setup_macos_platform_core()
endif() 