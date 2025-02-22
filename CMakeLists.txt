cmake_minimum_required(VERSION 3.10)

# Set policies if needed
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

project(InterviewCheater 
    VERSION 1.0.0
    DESCRIPTION "Interview Cheater Application"
    LANGUAGES CXX
)

# Set CMake to look in the cmake/ folder for additional scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


# All CMake variables defined in one place
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project root directory for convenience
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include(common-source)
include(help)
setup_help_target()

# Setup clang tools before adding targets
include(setup-clang-tools)
setup_clang_tools()

include(platform/windows)
include(platform/macos)
# Create the executable with basic sources
if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 
        ${CORE_SOURCES}
        ${COMMAND_SOURCES}
        ${SIGNAL_SOURCES}
        ${WINDOW_SOURCES}
        ${INPUT_SOURCES}
        ${PLATFORM_COMMON_SOURCES}
    )
    setup_windows_platform()
elseif(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE 
        ${CORE_SOURCES}
        ${COMMAND_SOURCES}
        ${SIGNAL_SOURCES}
        ${WINDOW_SOURCES}
        ${INPUT_SOURCES}
        ${PLATFORM_COMMON_SOURCES}
    )
    setup_macos_platform()
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Include format-lint after the target is created
include(format-lint)