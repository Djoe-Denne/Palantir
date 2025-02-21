function(setup_windows_platform)
    set(PLATFORM_SOURCES
        ${PROJECT_ROOT}/src/platform/windows/input/toggle_input.cpp
        ${PROJECT_ROOT}/src/platform/windows/input/stop_input.cpp
        ${PROJECT_ROOT}/src/platform/windows/signal/signal_manager.cpp
        ${PROJECT_ROOT}/src/platform/windows/window/overlay_window.cpp
        ${PROJECT_ROOT}/src/platform/windows/platform_application.cpp
        ${PROJECT_ROOT}/src/platform/windows/utils/logger.cpp
    )
    
    file(GLOB_RECURSE PLATFORM_HEADERS
        "${PROJECT_ROOT}/include/platform/windows/*.hpp"
        "${PROJECT_ROOT}/include/platform/windows/*.h"
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