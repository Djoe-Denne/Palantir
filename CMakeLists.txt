cmake_minimum_required(VERSION 3.10)

# Set the system name explicitly
if(WIN32)
    set(CMAKE_SYSTEM_NAME "Windows")
elseif(UNIX AND NOT APPLE)
    set(CMAKE_SYSTEM_NAME "Linux")
elseif(APPLE)
    set(CMAKE_SYSTEM_NAME "Darwin")
else()
    message(FATAL_ERROR "Unsupported target platform")
endif()

# Include CEF configuration
include(${CMAKE_SOURCE_DIR}/cef_cmake.cmake)

project(InterviewCheater)

set(CMAKE_CXX_STANDARD 17)

# Set compiler flags for Windows
if(MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    # Ensure consistent runtime library settings
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MDd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MD")
elseif(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
endif()

# Disable CEF sandbox to avoid runtime library conflicts
set(CEF_USE_SANDBOX OFF)

# Set the runtime output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Ensure CEF_CMAKE_OUTPUT_DIR or CMAKE_RUNTIME_OUTPUT_DIRECTORY is set
if(NOT DEFINED CEF_CMAKE_OUTPUT_DIR AND NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    message(FATAL_ERROR "Neither CEF_CMAKE_OUTPUT_DIR nor CMAKE_RUNTIME_OUTPUT_DIRECTORY is defined. Please set one of these variables.")
else()
    if(DEFINED CEF_CMAKE_OUTPUT_DIR)
        message(STATUS "CEF_CMAKE_OUTPUT_DIR is set to ${CEF_CMAKE_OUTPUT_DIR}")
    endif()
    if(DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
        message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY is set to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    endif()
endif()

# Conditionally add include/windows to the include path if building on Windows
if (WIN32)
    include_directories(include/windows)
endif()
# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES
    src/main.cpp
    src/command/*.cpp
    src/input/*.cpp
    src/signal/*.cpp
    src/window/*.cpp
    src/windows/signal/*.cpp
    src/windows/input/*.cpp
    src/windows/window/*.cpp
)

# Add CEF library
add_subdirectory(${CMAKE_SOURCE_DIR}/cef)

add_executable(InterviewCheater ${SOURCES})

# Link required Windows libraries and CEF
target_link_libraries(InterviewCheater PRIVATE gdi32 dwmapi user32 cefdll_wrapper)

# Add a post-build step to copy required DLLs to the output directory
if (WIN32)
    add_custom_command(TARGET InterviewCheater POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Redist/MSVC/14.28.29333/debug_nonredist/x64/Microsoft.VC142.DebugCRT/vcruntime140d.dll"
            "$<TARGET_FILE_DIR:InterviewCheater>/vcruntime140d.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Redist/MSVC/14.28.29333/debug_nonredist/x64/Microsoft.VC142.DebugCRT/vcruntime140_1d.dll"
            "$<TARGET_FILE_DIR:InterviewCheater>/vcruntime140_1d.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Redist/MSVC/14.28.29333/debug_nonredist/x64/Microsoft.VC142.DebugCRT/msvcp140d.dll"
            "$<TARGET_FILE_DIR:InterviewCheater>/msvcp140d.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Redist/MSVC/14.28.29333/debug_nonredist/x64/Microsoft.VC142.DebugCRT/ucrtbased.dll"
            "$<TARGET_FILE_DIR:InterviewCheater>/ucrtbased.dll"
        COMMENT "Copying required DLLs to the output directory"
    )
endif()

# Add macOS specific settings
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum OS X deployment version")
    
    # Enable @rpath for macOS builds
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "@executable_path")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()