set(WINDOWS_SOURCES
    ${PROJECT_ROOT}/application/src/platform/windows/platform_application.cpp
    ${PROJECT_ROOT}/application/src/platform/windows/window/overlay_window.cpp
)

set(COMMON_WINDOWS_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/platform/windows/input/configurable_input.cpp
    ${PROJECT_ROOT}/palantir-core/src/platform/windows/signal/signal_manager.cpp
    ${PROJECT_ROOT}/palantir-core/src/platform/windows/utils/logger.cpp
)

set(ALL_SOURCES
    ${ALL_SOURCES}
    ${WINDOWS_SOURCES}
)

function(setup_windows_platform_common target_name)
    # Windows-specific compile definitions
    target_compile_definitions(${target_name} PRIVATE
        UNICODE
        _UNICODE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
    
    target_link_libraries(${target_name} PRIVATE
        gdi32
        dwmapi
        user32
    )
endfunction()

function(setup_windows_platform)    
    target_sources(${PROJECT_NAME} PRIVATE 
        ${COMMON_WINDOWS_SOURCES}
        ${WINDOWS_SOURCES}
    )
    
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${COMMON_INCLUDE_DIRS}
        ${PROJECT_ROOT}/application/include/platform/windows
    )
    
    setup_windows_platform_common(${PROJECT_NAME})
endfunction()

function(setup_windows_platform_core)    
    target_sources(palantir-core PRIVATE
        ${COMMON_WINDOWS_SOURCES}
    )

    target_include_directories(palantir-core PRIVATE
        ${COMMON_INCLUDE_DIRS}
        ${PROJECT_ROOT}/application/include/platform/windows
    )
    setup_windows_platform_common(palantir-core)
endfunction() 