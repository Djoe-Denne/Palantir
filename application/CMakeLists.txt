# Create the executable with basic sources
if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 
        ${CORE_SOURCES}
        ${COMMAND_SOURCES}
        ${SIGNAL_SOURCES}
        ${WINDOW_SOURCES}
        ${INPUT_SOURCES}
        ${PLATFORM_COMMON_SOURCES}
    )
    setup_windows_platform()
elseif(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE 
        ${CORE_SOURCES}
        ${COMMAND_SOURCES}
        ${SIGNAL_SOURCES}
        ${WINDOW_SOURCES}
        ${INPUT_SOURCES}
        ${PLATFORM_COMMON_SOURCES}
    )
    setup_macos_platform()
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_INCLUDE_DIRS}) 