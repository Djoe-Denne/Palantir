set(MODULE_NAME "palantir-core")


if(NOT SAURON_SDK_FOUND)
    message(STATUS "System Sauron SDK not found, building from source...")
    # Find or install Sauron SDK
    include(FetchContent)
    FetchContent_Declare(
        SAURON_SDK
        GIT_REPOSITORY https://github.com/Djoe-Denne/Sauron-sdk.git
        GIT_BRANCH master
    )
    FetchContent_MakeAvailable(SAURON_SDK)
    
    # Variables are automatically set by the SDK
    # SAURON_SDK_INCLUDE_DIRS - Include directories
    # SAURON_SDK_LIBRARIES - Libraries to link against (sauron_sdk::curl)
else()
    message(STATUS "Found system Sauron SDK: ${SAURON_SDK_LIBRARIES}")
endif()

# Check for nlohmann_json
find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
    # If not found directly, it should be available through Sauron SDK
    # But we'll add a fallback just in case
    if(NOT TARGET nlohmann_json::nlohmann_json)
        message(STATUS "nlohmann_json not found directly, fetching it...")
        include(FetchContent)
        FetchContent_Declare(
            nlohmann_json
            GIT_REPOSITORY https://github.com/nlohmann/json.git
            GIT_TAG v3.11.2
        )
        FetchContent_MakeAvailable(nlohmann_json)
    endif()
endif()

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
    ${PROJECT_ROOT}/palantir-core/include/window/component/webview/webview.hpp
    ${PROJECT_ROOT}/palantir-core/include/window/component/content_manager.hpp
    ${PROJECT_ROOT}/palantir-core/include/command/icommand.hpp
    ${PROJECT_ROOT}/palantir-core/include/command/command_factory.hpp
    ${PROJECT_ROOT}/palantir-core/include/mode/debug/utils/logger.hpp
    ${PROJECT_ROOT}/palantir-core/include/input/key_register.hpp
)
setup_windows_platform_webview(${MODULE_NAME})
target_include_directories(${MODULE_NAME} PUBLIC ${COMMON_INCLUDE_DIRS})

# Link against Sauron SDK
if(TARGET sauron_sdk::curl)
    target_link_libraries(${MODULE_NAME} PRIVATE sauron_sdk::curl)
else()
    message(FATAL_ERROR "Sauron SDK target 'sauron_sdk::curl' not found. Please check your Sauron SDK installation.")
endif()

# Include Sauron SDK include directories
target_include_directories(${MODULE_NAME} PRIVATE ${SAURON_SDK_INCLUDE_DIRS}) 

# Make nlohmann_json available to consumers of this library
if(TARGET nlohmann_json::nlohmann_json)
    target_link_libraries(${MODULE_NAME} PUBLIC nlohmann_json::nlohmann_json)
else()
    # If using the version from Sauron SDK, we need to expose the include directories
    if(DEFINED NLOHMANN_JSON_INCLUDE_DIRS)
        target_include_directories(${MODULE_NAME} PUBLIC ${NLOHMANN_JSON_INCLUDE_DIRS})
    endif()
endif()

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

# Enable testing for this component
enable_testing()
add_subdirectory(tests) 