file(GLOB_RECURSE PROJECT_PALANTIR_HEADERS
    "${PROJECT_ROOT}/palantir-core/include/*.hpp"
    "${PROJECT_ROOT}/palantir-core/include/*.h"
)

set(COMMAND_PALANTIR_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/command/command_factory.cpp
)

set(CLIENT_PALANTIR_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/client/sauron_register.cpp
)

set(SIGNAL_PALANTIR_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/signal/signal.cpp
    ${PROJECT_ROOT}/palantir-core/src/signal/signal_factory.cpp
)

set(UTILS_PALANTIR_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/utils/resource_utils.cpp
)

set(WINDOW_PALANTIR_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/window/window_manager.cpp
    ${PROJECT_ROOT}/palantir-core/src/window/component/message/message_handler.cpp  
    ${PROJECT_ROOT}/palantir-core/src/window/component/message/logger/logger_strategy.cpp
    ${PROJECT_ROOT}/palantir-core/src/window/component/message/resize/resize_strategy.cpp
)

set(INPUT_PALANTIR_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/input/keyboard_input_factory.cpp
    ${PROJECT_ROOT}/palantir-core/src/input/key_config.cpp
    ${PROJECT_ROOT}/palantir-core/src/input/key_mapper.cpp
    ${PROJECT_ROOT}/palantir-core/src/input/key_register.cpp
)

set(APPLICATION_PALANTIR_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/application.cpp
)

# Debug/Release includes
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COMMON_PALANTIR_INCLUDES
        ${PROJECT_ROOT}/palantir-core/include/mode/debug
    )
    add_definitions(-DDEBUG)
else()
    set(COMMON_PALANTIR_INCLUDES
        ${PROJECT_ROOT}/palantir-core/include/mode/release
    )
endif()

# Common include directories for all platforms
set(COMMON_PALANTIR_INCLUDE_DIRS 
    ${PROJECT_ROOT}/palantir-core/include
    ${PROJECT_ROOT}/palantir-core/include/platform
    ${COMMON_PALANTIR_INCLUDES}
)

set(ALL_PALANTIR_INCLUDE_DIRS
    ${ALL_PALANTIR_INCLUDE_DIRS}
    ${COMMON_PALANTIR_INCLUDE_DIRS}
)

# Set ALL_PALANTIR_SOURCES variable for clang tools
set(ALL_PALANTIR_SOURCES
    ${COMMAND_PALANTIR_SOURCES}
    ${CLIENT_PALANTIR_SOURCES}
    ${SIGNAL_PALANTIR_SOURCES}
    ${WINDOW_PALANTIR_SOURCES}
    ${INPUT_PALANTIR_SOURCES}
    ${APPLICATION_PALANTIR_SOURCES}
    ${UTILS_PALANTIR_SOURCES}
) 

set(ALL_SOURCES
    ${ALL_SOURCES}
    ${ALL_PALANTIR_SOURCES} 
    ${PROJECT_PALANTIR_HEADERS}
    PARENT_SCOPE
)
