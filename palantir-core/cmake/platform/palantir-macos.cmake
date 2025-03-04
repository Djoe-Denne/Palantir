set(COMMON_MACOS_PALANTIR_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/platform/macos/input/configurable_input.mm
    ${PROJECT_ROOT}/palantir-core/src/platform/macos/signal/signal_manager.mm
    ${PROJECT_ROOT}/palantir-core/src/platform/macos/utils/logger.mm
)

set(ALL_PALANTIR_SOURCES
    ${ALL_PALANTIR_SOURCES}
    ${MACOS_PALANTIR_SOURCES}
    ${MACOS_PALANTIR_HEADERS}
)

set(ALL_SOURCES
    ${ALL_SOURCES}
    ${ALL_PALANTIR_SOURCES}
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

function(setup_macos_platform_core)
    target_sources(palantir-core PRIVATE
        ${COMMON_MACOS_PALANTIR_SOURCES}
    )

    target_include_directories(palantir-core PRIVATE
        ${COMMON_PALANTIR_INCLUDE_DIRS}
        ${PROJECT_ROOT}/palantir-core/include/platform/macos
    )
    setup_macos_platform_common(palantir-core)
endfunction() 