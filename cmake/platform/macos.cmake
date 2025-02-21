function(setup_macos_platform)
    set(PLATFORM_SOURCES
        ${PROJECT_ROOT}/src/platform/macos/input/toggle_input.mm
        ${PROJECT_ROOT}/src/platform/macos/input/stop_input.mm
        ${PROJECT_ROOT}/src/platform/macos/input/configurable_input.mm
        ${PROJECT_ROOT}/src/platform/macos/signal/signal_manager.cpp
        ${PROJECT_ROOT}/src/platform/macos/window/overlay_window.mm
        ${PROJECT_ROOT}/src/platform/macos/platform_application.mm
        ${PROJECT_ROOT}/src/platform/macos/utils/logger.mm
    )
    
    file(GLOB_RECURSE PLATFORM_HEADERS
        "${PROJECT_ROOT}/include/platform/macos/*.hpp"
        "${PROJECT_ROOT}/include/platform/macos/*.h"
    )

    # Update ALL_SOURCES in parent scope
    set(ALL_SOURCES
        ${ALL_SOURCES}
        ${PLATFORM_SOURCES}
        ${PLATFORM_HEADERS}
        PARENT_SCOPE
    )
    
    target_sources(${PROJECT_NAME} PRIVATE 
        ${PLATFORM_SOURCES}
        ${PLATFORM_HEADERS}
    )
    
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${COMMON_INCLUDE_DIRS}
        ${PROJECT_ROOT}/include/platform/macos
    )

    # macOS-specific compile options
    target_compile_options(${PROJECT_NAME} PRIVATE
        -fobjc-arc  # Enable Automatic Reference Counting
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${PROJECT_ROOT}/Info.plist"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
        XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED NO
        XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME YES
    )
    
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(CARBON_LIBRARY Carbon REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${COCOA_LIBRARY}
        ${CARBON_LIBRARY}
    )
endfunction() 