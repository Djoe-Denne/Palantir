file(GLOB_RECURSE PROJECT_HEADERS
    "${PROJECT_ROOT}/application/include/*.hpp"
    "${PROJECT_ROOT}/application/include/*.h"
)

# Project structure variables
set(CORE_SOURCES
    ${PROJECT_ROOT}/application/src/main.cpp
)

set(COMMAND_SOURCES
    ${PROJECT_ROOT}/application/src/command/command_factory.cpp
    ${PROJECT_ROOT}/application/src/command/show_command.cpp
    ${PROJECT_ROOT}/application/src/command/stop_command.cpp
)

set(SIGNAL_SOURCES
    ${PROJECT_ROOT}/application/src/signal/signal.cpp
    ${PROJECT_ROOT}/application/src/signal/signal_factory.cpp
)

set(WINDOW_SOURCES
    ${PROJECT_ROOT}/application/src/window/window_manager.cpp
)

set(INPUT_SOURCES
    ${PROJECT_ROOT}/application/src/input/input_factory.cpp
    ${PROJECT_ROOT}/application/src/input/key_config.cpp
    ${PROJECT_ROOT}/application/src/input/key_mapper.cpp
    ${PROJECT_ROOT}/application/src/input/key_register.cpp
)

set(PLATFORM_COMMON_SOURCES
    ${PROJECT_ROOT}/application/src/platform/application.cpp
)

# Debug/Release includes
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COMMON_INCLUDES
        ${PROJECT_ROOT}/application/include/mode/debug
    )
    add_definitions(-DDEBUG)
else()
    set(COMMON_INCLUDES
        ${PROJECT_ROOT}/application/include/mode/release
    )
endif()

# Common include directories for all platforms
set(COMMON_INCLUDE_DIRS
    ${PROJECT_ROOT}/application/include
    ${PROJECT_ROOT}/application/include/platform
    ${COMMON_INCLUDES}
)

# Set ALL_SOURCES variable for clang tools
set(ALL_SOURCES
    ${CORE_SOURCES}
    ${COMMAND_SOURCES}
    ${SIGNAL_SOURCES}
    ${WINDOW_SOURCES}
    ${INPUT_SOURCES}
    ${PLATFORM_COMMON_SOURCES}
) 