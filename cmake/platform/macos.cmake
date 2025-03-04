set(MACOS_APPLICATION_SOURCES
    ${PROJECT_ROOT}/application/src/platform/macos/platform_application.mm
    ${PROJECT_ROOT}/application/src/platform/macos/window/overlay_window.mm
)

file(GLOB_RECURSE MACOS_APPLICATION_HEADERS
    "${PROJECT_ROOT}/application/include/platform/macos/*.hpp"
    "${PROJECT_ROOT}/application/include/platform/macos/*.h"
)

set(ALL_APPLICATION_SOURCES
    ${ALL_APPLICATION_SOURCES}
    ${MACOS_APPLICATION_SOURCES}
    ${MACOS_APPLICATION_HEADERS}
)

set(ALL_SOURCES
    ${ALL_SOURCES}
    ${ALL_APPLICATION_SOURCES}
)

function(setup_macos_platform_common target_name)
    # macOS-specific compile options
    target_compile_options(${target_name} PRIVATE
        -fobjc-arc  # Enable Automatic Reference Counting
    )

    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(CARBON_LIBRARY Carbon REQUIRED)
    target_link_libraries(${target_name} PRIVATE
        ${COCOA_LIBRARY}
        ${CARBON_LIBRARY}
    )
endfunction()

function(setup_macos_platform)
    target_sources(${PROJECT_NAME} PRIVATE 
        ${MACOS_APPLICATION_SOURCES}
        ${COMMON_MACOS_APPLICATION_SOURCES}
        ${MACOS_APPLICATION_HEADERS}
    )
    
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${COMMON_INCLUDE_DIRS}
        ${PROJECT_ROOT}/application/include/platform/macos
    )

    setup_macos_platform_common(${PROJECT_NAME})

    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${PROJECT_ROOT}/Info.plist"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
        XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED NO
        XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME YES
    )
endfunction()
