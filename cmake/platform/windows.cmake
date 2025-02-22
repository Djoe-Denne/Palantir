
set(WINDOWS_SOURCES
    ${PROJECT_ROOT}/src/platform/windows/input/configurable_input.cpp
    ${PROJECT_ROOT}/src/platform/windows/signal/signal_manager.cpp
    ${PROJECT_ROOT}/src/platform/windows/window/overlay_window.cpp
    ${PROJECT_ROOT}/src/platform/windows/platform_application.cpp
    ${PROJECT_ROOT}/src/platform/windows/utils/logger.cpp
)

file(GLOB_RECURSE WINDOWS_HEADERS
    "${PROJECT_ROOT}/include/platform/windows/*.hpp"
    "${PROJECT_ROOT}/include/platform/windows/*.h"
)

set(ALL_SOURCES
    ${ALL_SOURCES}
    ${WINDOWS_SOURCES}
    ${WINDOWS_HEADERS}
)

function(setup_windows_platform)    
    target_sources(${PROJECT_NAME} PRIVATE 
        ${WINDOWS_SOURCES}
        ${WINDOWS_HEADERS}
    )
    
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${COMMON_INCLUDE_DIRS}
        ${PROJECT_ROOT}/include/platform/windows
    )
    
    # Windows-specific compile definitions
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        UNICODE
        _UNICODE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
    
    target_link_libraries(${PROJECT_NAME} PRIVATE
        gdi32
        dwmapi
        user32
    )
endfunction() 