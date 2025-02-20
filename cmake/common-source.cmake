file(GLOB_RECURSE PROJECT_HEADERS
    "${PROJECT_ROOT}/include/*.hpp"
    "${PROJECT_ROOT}/include/*.h"
)

# Project structure variables
set(CORE_SOURCES
    ${PROJECT_ROOT}/src/main.cpp
)

set(COMMAND_SOURCES
    ${PROJECT_ROOT}/src/command/show_command.cpp
)

set(SIGNAL_SOURCES
    ${PROJECT_ROOT}/src/signal/toggle_signal.cpp
    ${PROJECT_ROOT}/src/signal/signal_manager.cpp
)

set(WINDOW_SOURCES
    ${PROJECT_ROOT}/src/window/window_manager.cpp
)

set(INPUT_SOURCES
    ${PROJECT_ROOT}/src/input/input_factory.cpp
)

set(PLATFORM_COMMON_SOURCES
    ${PROJECT_ROOT}/src/platform/application.cpp
)

# Debug/Release includes
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COMMON_INCLUDES
        ${PROJECT_ROOT}/include/mode/debug
    )
    add_definitions(-DDEBUG)
else()
    set(COMMON_INCLUDES
        ${PROJECT_ROOT}/include/mode/release
    )
endif()

# Common include directories for all platforms
set(COMMON_INCLUDE_DIRS
    ${PROJECT_ROOT}/include
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
    ${PROJECT_HEADERS}
) 