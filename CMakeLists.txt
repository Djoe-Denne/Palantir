cmake_minimum_required(VERSION 3.10)

project(InterviewCheater)

set(CMAKE_CXX_STANDARD 17)

# Determine the platform-specific source directory
if(WIN32)
    set(PLATFORM_DIR "windows")
elseif(APPLE)
    set(PLATFORM_DIR "macos")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Common include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/platform/${PLATFORM_DIR}
)

# Common source files
file(GLOB_RECURSE COMMON_SOURCES
    src/main.cpp
    src/command/*.cpp
    src/window/*.cpp
    src/signal/*.cpp
    src/input/*.cpp
    src/platform/*.cpp
)

# Platform-specific source files
file(GLOB_RECURSE PLATFORM_SOURCES
    src/platform/${PLATFORM_DIR}/*.cpp
    src/platform/${PLATFORM_DIR}/*.mm
)

set(SOURCES ${COMMON_SOURCES} ${PLATFORM_SOURCES})

# Platform-specific configuration
if(APPLE)
    # macOS configuration
    set(CMAKE_MACOSX_RPATH 1)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks")
    
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES})
    
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
        XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED NO
    )
    
    # macOS frameworks
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(CARBON_LIBRARY Carbon REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${COCOA_LIBRARY}
        ${CARBON_LIBRARY}
    )
else()
    # Windows configuration
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
    
    target_link_libraries(${PROJECT_NAME} PRIVATE
        gdi32
        dwmapi
        user32
    )
    
    # Copy Windows DLLs
    if(MSVC)
        set(MSVC_REDIST_DIR "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Redist/MSVC/14.28.29333/debug_nonredist/x64/Microsoft.VC142.DebugCRT")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${MSVC_REDIST_DIR}/vcruntime140d.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/vcruntime140d.dll"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${MSVC_REDIST_DIR}/vcruntime140_1d.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/vcruntime140_1d.dll"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${MSVC_REDIST_DIR}/msvcp140d.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/msvcp140d.dll"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${MSVC_REDIST_DIR}/ucrtbased.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/ucrtbased.dll"
            COMMENT "Copying required DLLs to the output directory"
        )
    endif()
endif()