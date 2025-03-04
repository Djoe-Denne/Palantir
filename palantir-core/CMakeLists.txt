set(MODULE_NAME "palantir-core")


# Set CMake to look in the cmake/ folder for additional scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/palantir-core/cmake")

include(install-palantir-deps)
include(palantir-common-source)
include(platform/palantir-windows)
include(platform/palantir-macos)

install_palantir_deps()

# Create the shared library with basic sources
add_library(${MODULE_NAME} SHARED
    ${ALL_PALANTIR_SOURCES}
)
target_include_directories(${MODULE_NAME} PUBLIC ${ALL_PALANTIR_INCLUDE_DIRS})

target_link_libraries(${MODULE_NAME} PRIVATE sauron_sdk::curl)

# Include Sauron SDK include directories
target_include_directories(${MODULE_NAME} PUBLIC ${SAURON_SDK_INCLUDE_DIRS}) 

target_link_libraries(${MODULE_NAME} PUBLIC nlohmann_json::nlohmann_json)

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
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests) 
endif()