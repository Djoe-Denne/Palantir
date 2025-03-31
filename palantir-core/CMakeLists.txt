set(MODULE_NAME "palantir-core")


# Set CMake to look in the cmake/ folder for additional scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/palantir-core/cmake")

include(palantir-common-source)

if(WIN32 OR QUALITY_ONLY)
    include(platform/palantir-windows)
endif()
if(APPLE)
    include(platform/palantir-macos)
endif()

if(NOT QUALITY_ONLY)
    include(install-palantir-deps)
    install_palantir_deps()
endif()


# Create the shared library with basic sources
add_library(${MODULE_NAME} SHARED
    ${ALL_PALANTIR_SOURCES}
)

if(NOT QUALITY_ONLY)
    target_include_directories(${MODULE_NAME} PUBLIC ${ALL_PALANTIR_INCLUDE_DIRS})

    target_link_libraries(${MODULE_NAME} PRIVATE sauron_sdk::curl)

    # Include Sauron SDK include directories
    target_include_directories(${MODULE_NAME} PUBLIC ${SAURON_SDK_INCLUDE_DIRS}) 

    target_link_libraries(${MODULE_NAME} PUBLIC nlohmann_json::nlohmann_json)

    target_compile_definitions(${MODULE_NAME} PUBLIC PALANTIR_CORE_EXPORTS)
endif()


# Set output directories for the library
set_target_properties(${MODULE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)


# Enable testing for this component
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests) 
endif()