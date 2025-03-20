set(COMMON_WINDOWS_PALANTIR_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/platform/windows/input/keyboard_Input.cpp
    ${PROJECT_ROOT}/palantir-core/src/platform/windows/signal/signal_manager.cpp
    ${PROJECT_ROOT}/palantir-core/src/platform/windows/utils/logger.cpp
)

set(ALL_PALANTIR_SOURCES
    ${ALL_PALANTIR_SOURCES}
    ${WINDOWS_PALANTIR_SOURCES}
)

set(ALL_SOURCES
    ${ALL_SOURCES}
    ${ALL_PALANTIR_SOURCES}
)

function(setup_windows_platform_core)
    target_sources(palantir-core PRIVATE
        ${COMMON_WINDOWS_PALANTIR_SOURCES}
    )

    target_include_directories(palantir-core PRIVATE
        ${COMMON_PALANTIR_INCLUDE_DIRS}
    )
    setup_windows_platform_common(palantir-core)
endfunction() 

