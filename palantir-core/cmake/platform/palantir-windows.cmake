set(WINDOWS_PALANTIR_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/platform/windows/input/keyboard_input.cpp
    ${PROJECT_ROOT}/palantir-core/src/platform/windows/utils/logger.cpp
)

set(WINDOWS_PALANTIR_INCLUDES_DIRS
    ${PROJECT_ROOT}/palantir-core/include/platform/windows
)   

set(ALL_PALANTIR_INCLUDE_DIRS
    ${ALL_PALANTIR_INCLUDE_DIRS}
    ${WINDOWS_PALANTIR_INCLUDES_DIRS}
)

set(ALL_PALANTIR_SOURCES
    ${ALL_PALANTIR_SOURCES}
    ${WINDOWS_PALANTIR_SOURCES}
)

set(ALL_SOURCES
    ${ALL_SOURCES}
    ${ALL_PALANTIR_SOURCES}
)


