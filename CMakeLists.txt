cmake_minimum_required(VERSION 3.10)

# waiting for nlohmann/json to update CMake minimum version and create tag
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

option(BUILD_TESTS "Build tests" OFF)
# Option to control automatic installation
option(AUTO_INSTALL_MISSING_TOOLS "Automatically try to install missing clang tools via package managers" ON)
option(MAGIC_DEPS_INSTALL "Try to install missing dependencies via package managers" ON)
option(QUALITY_ONLY "Build only quality tools" OFF)
option(FILES_TO_LINT "Files to lint" "")

# Set policies if needed
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

project(Palantir 
    VERSION 1.0.0
    DESCRIPTION "Palantir Application"
    LANGUAGES CXX
)

# Enable testing at the top level
include(CTest)
enable_testing()

# Set CMake to look in the cmake/ folder for additional scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


# All CMake variables defined in one place
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(setup-modern-exception)
setup_modern_exception()

# Project root directory for convenience
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include(common-source)
include(help)
setup_help_target()

# Setup clang tools before adding targets
include(setup-clang-tools)
setup_clang_tools()


if(WIN32 OR QUALITY_ONLY)
    include(platform/windows)
endif()
if(APPLE)
    include(platform/macos)
endif()

# Setup Google Test
include(setup-gtest)

# Add the palantir-core subdirectory
add_subdirectory(palantir-core)

# Add the plugin-loader subdirectory
add_subdirectory(plugin-loader)

# Add the plugins subdirectory
add_subdirectory(plugins/commands)

# Add the application subdirectory
add_subdirectory(application)

# Include format-lint after the target is created
include(format-lint)

# Install resource directory
install(DIRECTORY ${CMAKE_SOURCE_DIR}/resource
        DESTINATION .
        COMPONENT resources)
