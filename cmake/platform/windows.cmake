function(setup_windows_platform)
    set(PLATFORM_SOURCES
        ${PROJECT_ROOT}/src/platform/windows/input/input.cpp
        ${PROJECT_ROOT}/src/platform/windows/window/overlay_window.cpp
        ${PROJECT_ROOT}/src/platform/windows/platform_application.cpp
        ${PROJECT_ROOT}/src/platform/windows/utils/logger.cpp
    )
    
    target_sources(${PROJECT_NAME} PRIVATE ${PLATFORM_SOURCES})
    
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