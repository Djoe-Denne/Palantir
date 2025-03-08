include(FetchContent)

# Helper function to setup testing for a target
function(setup_target_testing TARGET_NAME)
    set(INSTALL_GTEST ON CACHE BOOL "" FORCE)
    set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
    find_package(googletest QUIET)
    if(NOT googletest_FOUND AND MAGIC_DEPS_INSTALL)
        message(STATUS "GTest not found. Setting up GTest via FetchContent...")
        
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.16.0
        )
        
        # Windows-specific settings
        message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
        if(WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
            message(STATUS "Setting up GTest for Windows")
            set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
            # Ensure DLL compatibility
            set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
            add_compile_definitions(GTEST_CREATE_SHARED_LIBRARY=1)
        endif()

        FetchContent_MakeAvailable(googletest)
        
        message(STATUS "GTest setup complete.")

        # Set variables to match find_package behavior
        set(googletest_FOUND TRUE CACHE BOOL "GTest found")
        set(googletest_INCLUDE_DIRS ${gtest_SOURCE_DIR}/include CACHE PATH "GTest include directories")
        set(googletest_mock_INCLUDE_DIRS ${gmock_SOURCE_DIR}/include CACHE PATH "GMock include directories")
        set(googletest_LIBRARIES gtest CACHE FILEPATH "GTest libraries")
        set(googletest_main_LIBRARIES gtest_main CACHE FILEPATH "GTest main libraries")
        set(googletest_mock_LIBRARIES gmock CACHE FILEPATH "GMock libraries")
        set(googletest_mock_main_LIBRARIES gmock_main CACHE FILEPATH "GMock main libraries")
        
        message(STATUS "GTest include dirs: ${googletest_INCLUDE_DIRS}")
        message(STATUS "GMock include dirs: ${googletest_mock_INCLUDE_DIRS}")
        message(STATUS "GTest libraries: ${googletest_LIBRARIES}")
        message(STATUS "GTest main libraries: ${googletest_main_LIBRARIES}")
        message(STATUS "GMock libraries: ${googletest_mock_LIBRARIES}")
        message(STATUS "GMock main libraries: ${googletest_mock_main_LIBRARIES}")
    elseif(NOT googletest_FOUND AND NOT AUTO_INSTALL_GTEST)
        message(WARNING "GTest not found and AUTO_INSTALL_GTEST is OFF. Tests will be disabled.")
        return()
    endif()

    message(STATUS "Setting up testing for target ${TARGET_NAME}")
    
    if(WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
        message(STATUS "Setting up GTest for Windows")
        target_compile_definitions(${TARGET_NAME}
            PRIVATE
                GTEST_LINKED_AS_SHARED_LIBRARY=1
        )
    endif()
    
    target_link_libraries(${TARGET_NAME}
        PRIVATE
            ${googletest_LIBRARIES}
            ${googletest_main_LIBRARIES}
            ${googletest_mock_LIBRARIES}
            ${googletest_mock_main_LIBRARIES}
    )
    
    target_include_directories(${TARGET_NAME}
        PRIVATE
            ${googletest_INCLUDE_DIRS}
            ${googletest_mock_INCLUDE_DIRS}
    )
endfunction() 