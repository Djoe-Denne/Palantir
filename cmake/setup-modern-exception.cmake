function(setup_modern_exception)    
    message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
    message(STATUS "Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
    # Check compiler ID and version
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "13")
            message(STATUS "Enabling stacktrace support for GCC")
            set(STACKTRACE_SUPPORT_ENABLED 1)
            add_compile_definitions(_GLIBCXX_HAVE_STACKTRACE=1)
            set(CMAKE_CXX_STANDARD 23)
        endif()
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "19.35") # VS 2022 version 17.5
            message(STATUS "Enabling stacktrace support for MSVC")
            set(STACKTRACE_SUPPORT_ENABLED 1)
            add_compile_definitions(_GLIBCXX_HAVE_STACKTRACE=1)
            set(CMAKE_CXX_STANDARD 23)
            # Enable latest C++ features for MSVC
            add_compile_options(/std:c++latest)
        endif()
    endif()

    # Set C++ standard if not already set by compiler-specific checks
    if(NOT CMAKE_CXX_STANDARD)
        set(CMAKE_CXX_STANDARD 20)
    endif()
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
    message(STATUS "C++ Extensions: ${CMAKE_CXX_EXTENSIONS}")
    message(STATUS "C++ Standard Required: ${CMAKE_CXX_STANDARD_REQUIRED}")
    # Optional: Print status message about stacktrace support
    if(STACKTRACE_SUPPORT_ENABLED)
        message(STATUS "Stacktrace support enabled")
    else()
        message(STATUS "Stacktrace support not available with current compiler")
    endif()
endfunction()