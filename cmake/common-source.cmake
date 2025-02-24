file(GLOB_RECURSE PROJECT_HEADERS
    "${PROJECT_ROOT}/palantir-core/include/*.hpp"
    "${PROJECT_ROOT}/palantir-core/include/*.h"
)


set(COMMAND_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/command/command_factory.cpp
    ${PROJECT_ROOT}/palantir-core/src/command/show_command.cpp
    ${PROJECT_ROOT}/palantir-core/src/command/stop_command.cpp
)

set(SIGNAL_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/signal/signal.cpp
    ${PROJECT_ROOT}/palantir-core/src/signal/signal_factory.cpp
)

set(WINDOW_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/window/window_manager.cpp
)

set(INPUT_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/input/input_factory.cpp
    ${PROJECT_ROOT}/palantir-core/src/input/key_config.cpp
    ${PROJECT_ROOT}/palantir-core/src/input/key_mapper.cpp
    ${PROJECT_ROOT}/palantir-core/src/input/key_register.cpp
)

set(APPLICATION_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/application.cpp
)

# Debug/Release includes
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COMMON_INCLUDES
        ${PROJECT_ROOT}/palantir-core/include/mode/debug
    )
    add_definitions(-DDEBUG)
else()
    set(COMMON_INCLUDES
        ${PROJECT_ROOT}/palantir-core/include/mode/release
    )
endif()

# Common include directories for all platforms
set(COMMON_INCLUDE_DIRS
    ${PROJECT_ROOT}/palantir-core/include
    ${PROJECT_ROOT}/palantir-core/include/platform
    ${PROJECT_ROOT}/application/include/platform
    ${COMMON_INCLUDES}
)

# Set ALL_SOURCES variable for clang tools
set(ALL_SOURCES
    ${COMMAND_SOURCES}
    ${SIGNAL_SOURCES}
    ${WINDOW_SOURCES}
    ${INPUT_SOURCES}
    ${APPLICATION_SOURCES}
) 