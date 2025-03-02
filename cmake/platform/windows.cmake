set(WINDOWS_SOURCES
    ${PROJECT_ROOT}/application/src/platform/windows/platform_application.cpp
    ${PROJECT_ROOT}/application/src/platform/windows/window/overlay_window.cpp
    ${PROJECT_ROOT}/application/src/platform/windows/window/overlay_window_impl.cpp
    ${PROJECT_ROOT}/application/src/platform/windows/window/component/webview/webview.cpp
)

set(COMMON_WINDOWS_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/platform/windows/input/configurable_input.cpp
    ${PROJECT_ROOT}/palantir-core/src/platform/windows/signal/signal_manager.cpp
    ${PROJECT_ROOT}/palantir-core/src/platform/windows/utils/logger.cpp
)

set(ALL_SOURCES
    ${ALL_SOURCES}
    ${WINDOWS_SOURCES}
)

function(setup_windows_platform_common target_name)
    # Windows-specific compile definitions
    target_compile_definitions(${target_name} PRIVATE
        UNICODE
        _UNICODE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
    
    target_link_libraries(${target_name} PRIVATE
        gdi32
        dwmapi
        user32
    )
endfunction()

function(setup_windows_platform)    
    setup_windows_platform_webview(${PROJECT_NAME})
    target_sources(${PROJECT_NAME} PRIVATE 
        ${COMMON_WINDOWS_SOURCES}
        ${WINDOWS_SOURCES}
    )
    
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${COMMON_INCLUDE_DIRS}
        ${PROJECT_ROOT}/application/include/platform/windows
    )
    
    setup_windows_platform_common(${PROJECT_NAME})
endfunction()

function(setup_windows_platform_core)    
    target_sources(palantir-core PRIVATE
        ${COMMON_WINDOWS_SOURCES}
    )

    target_include_directories(palantir-core PRIVATE
        ${COMMON_INCLUDE_DIRS}
        ${PROJECT_ROOT}/application/include/platform/windows
    )
    setup_windows_platform_common(palantir-core)
endfunction() 

function(setup_vcpkg)
    find_program(VCPKG_EXECUTABLE vcpkg)

    if (VCPKG_EXECUTABLE)
        message(STATUS "Found vcpkg: ${VCPKG_EXECUTABLE}")

        # Extract the vcpkg root directory (remove '/vcpkg.exe' from the path)
        get_filename_component(VCPKG_ROOT "${VCPKG_EXECUTABLE}" DIRECTORY)
        message(STATUS "Detected vcpkg root directory: ${VCPKG_ROOT}")

        if (EXISTS "${VCPKG_ROOT}")
            # Set the vcpkg toolchain file only if not already set
            if (NOT CMAKE_TOOLCHAIN_FILE)
                set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file" FORCE)
                message(STATUS "Using vcpkg toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
            else()
                message(STATUS "CMAKE_TOOLCHAIN_FILE is already set to: ${CMAKE_TOOLCHAIN_FILE}")
            endif()

            # Append vcpkg installed directory to CMAKE_PREFIX_PATH
            list(APPEND CMAKE_PREFIX_PATH "${VCPKG_ROOT}/installed/x64-windows")
            set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" CACHE STRING "Vcpkg installed libraries path" FORCE)

            message(STATUS "Vcpkg setup complete!")
        else()
            message(WARNING "Vcpkg root directory not found! Please ensure vcpkg is correctly installed.")
        endif()

    else()
        message(WARNING "vcpkg executable not found! You must install vcpkg manually and add it to your PATH.")
    endif()
endfunction()

function(setup_windows_platform_webview target_name)
    find_package(webview2 QUIET)
    if(NOT webview2_FOUND)
        message(STATUS "Webview2 not found! Fetching from nuget...")
        # Microsoft.Web.WebView2
        FetchContent_Declare(
            webview2
            URL  https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/1.0.3065.39
        )
        FetchContent_MakeAvailable(webview2)
        message(STATUS "Webview2 fetched from nuget!")
    endif()

    message(STATUS "Webview2 source dir: ${webview2_SOURCE_DIR}")
    
    target_include_directories(${target_name} PUBLIC "${webview2_SOURCE_DIR}/build/native/include/")

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(PLATFORM "x64")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(PLATFORM "x86")
    endif()
    
    # Check if the WebView2 library exists at the expected path
    set(WEBVIEW2_LIB_PATH "${webview2_SOURCE_DIR}/build/native/${PLATFORM}/WebView2LoaderStatic.lib")
    if(NOT EXISTS "${WEBVIEW2_LIB_PATH}")
        # Try to find the library in a different location
        file(GLOB_RECURSE WEBVIEW2_LIB_FILES "${webview2_SOURCE_DIR}/*WebView2LoaderStatic.lib")
        if(WEBVIEW2_LIB_FILES)
            list(GET WEBVIEW2_LIB_FILES 0 WEBVIEW2_LIB_PATH)
            message(STATUS "Found WebView2 library at: ${WEBVIEW2_LIB_PATH}")
        else()
            message(FATAL_ERROR "WebView2 library not found. Expected at: ${WEBVIEW2_LIB_PATH}")
        endif()
    else()
        message(STATUS "WebView2 library found at: ${WEBVIEW2_LIB_PATH}")
    endif()
    
    target_link_libraries(${target_name} PRIVATE "${WEBVIEW2_LIB_PATH}")
endfunction()
