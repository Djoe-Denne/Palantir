cmake_minimum_required(VERSION 3.10)

project(InterviewCheater)

set(CMAKE_CXX_STANDARD 17)

# Define source groups
set(CORE_SOURCES
    src/main.cpp
)

set(COMMAND_SOURCES
    src/command/show_command.cpp
)

set(SIGNAL_SOURCES
    src/signal/toggle_signal.cpp
    src/signal/signal_manager.cpp
)

set(WINDOW_SOURCES
    src/window/window_manager.cpp
)

set(INPUT_SOURCES
    src/input/input_factory.cpp
)

set(PLATFORM_COMMON_SOURCES
    src/platform/application.cpp
)

# Platform-specific configuration and sources
if(WIN32)
    set(PLATFORM_SOURCES
        src/platform/windows/input.cpp
        src/platform/windows/window/overlay_window.cpp
        src/platform/windows/windows_application.cpp
    )
    
    add_executable(${PROJECT_NAME} WIN32 
        ${CORE_SOURCES}
        ${COMMAND_SOURCES}
        ${SIGNAL_SOURCES}
        ${WINDOW_SOURCES}
        ${INPUT_SOURCES}
        ${PLATFORM_COMMON_SOURCES}
        ${PLATFORM_SOURCES}
    )
    
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/platform/windows
    )
    
    target_link_libraries(${PROJECT_NAME} PRIVATE
        gdi32
        dwmapi
        user32
    )
elseif(APPLE)
    set(PLATFORM_SOURCES
        src/platform/macos/input.mm
        src/platform/macos/window/overlay_window.mm
        src/platform/macos/macos_application.mm
    )
    
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE 
        ${CORE_SOURCES}
        ${COMMAND_SOURCES}
        ${SIGNAL_SOURCES}
        ${WINDOW_SOURCES}
        ${INPUT_SOURCES}
        ${PLATFORM_COMMON_SOURCES}
        ${PLATFORM_SOURCES}
    )
    
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/platform/macos
    )
    
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
        XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED NO
    )
    
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(CARBON_LIBRARY Carbon REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${COCOA_LIBRARY}
        ${CARBON_LIBRARY}
    )
else()
    message(FATAL_ERROR "Unsupported platform")
endif()