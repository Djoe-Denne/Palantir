# Create the executable with basic sources
if(WIN32 OR QUALITY_ONLY)
    add_executable(${PROJECT_NAME} WIN32 
        ${WINDOWS_SOURCES}
        ${PROJECT_ROOT}/application/src/main.cpp
    )
    setup_windows_platform()
elseif(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE 
        ${MACOS_SOURCES}
        ${MACOS_HEADERS}
        ${PROJECT_ROOT}/application/src/main.cpp
    )
    setup_macos_platform()
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Set output directory for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Let CMake know where to search for the library
link_directories(${CMAKE_BINARY_DIR}/lib)

target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_INCLUDE_DIRS})

# Link against palantir-core and plugin-loader
target_link_libraries(${PROJECT_NAME} PRIVATE 
    palantir-core
    plugin-loader
)

# Enable testing for this component
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()