file(GLOB_RECURSE PROJECT_APPLICATIONHEADERS
    "${PROJECT_ROOT}/apPlication/include/*.hpp"
    "${PROJECT_ROOT}/application/include/*.h"
)

# Common include directories for all platforms
set(COMMON_INCLUDE_DIRS
    ${PROJECT_ROOT}/application/include
    ${PROJECT_ROOT}/application/include/platform
    ${COMMON_INCLUDES}
)

set(WINDOW_APPLICATION_SOURCES
    ${WINDOW_APPLICATION_SOURCES}
    ${PROJECT_ROOT}/application/src/window/overlay_window.cpp
    ${PROJECT_ROOT}/application/src/platform/windows/window/component/webview/webview_callbacks.cpp
)

# Set ALL_APPLICATION_SOURCES variable for clang tools
set(ALL_APPLICATION_SOURCES
    ${COMMAND_APPLICATION_SOURCES}
    ${CLIENT_APPLICATION_SOURCES}
    ${SIGNAL_APPLICATION_SOURCES}
    ${WINDOW_APPLICATION_SOURCES}
    ${INPUT_APPLICATION_SOURCES}
    ${APPLICATION_APPLICATION_SOURCES}
) 

set(ALL_SOURCES
    ${ALL_SOURCES}
    ${ALL_APPLICATION_SOURCES}
    ${PROJECT_APPLICATION_HEADERS}
)
