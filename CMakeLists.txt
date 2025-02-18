cmake_minimum_required(VERSION 3.10)

project(InterviewCheater)

set(CMAKE_CXX_STANDARD 17)

# Common include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Common source files (platform-independent)
set(COMMON_SOURCES
    src/main.cpp
    src/command/show_command.cpp
    src/signal/toggle_signal.cpp
    src/signal/signal_manager.cpp
    src/window/window_manager.cpp
    src/input/input_factory.cpp
    src/platform/application.cpp
)

# Platform-specific configuration and sources
if(WIN32)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/platform/windows)
    set(PLATFORM_SOURCES
        src/platform/windows/input.cpp
        src/platform/windows/window/overlay_window.cpp
        src/platform/windows/windows_application.cpp
    )
    
    add_executable(${PROJECT_NAME} WIN32 ${COMMON_SOURCES} ${PLATFORM_SOURCES})
    target_link_libraries(${PROJECT_NAME} PRIVATE gdi32 dwmapi user32)
elseif(APPLE)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/platform/macos)
    set(PLATFORM_SOURCES
        src/platform/macos/input.mm
        src/platform/macos/window/overlay_window.mm
        src/platform/macos/macos_application.mm
    )
    
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${COMMON_SOURCES} ${PLATFORM_SOURCES})
    
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
        XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED NO
    )
    
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(CARBON_LIBRARY Carbon REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COCOA_LIBRARY} ${CARBON_LIBRARY})
else()
    message(FATAL_ERROR "Unsupported platform")
endif()