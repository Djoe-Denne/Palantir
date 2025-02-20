function(setup_clang_tools)
    # Option to control automatic installation
    option(AUTO_INSTALL_MISSING_TOOLS "Automatically try to install missing clang tools via package managers" ON)

    # Allow manual override of clang tool paths via CMake arguments
    set(CLANG_FORMAT_PATH "" CACHE PATH "Manual path to clang-format executable")
    set(CLANG_TIDY_PATH "" CACHE PATH "Manual path to clang-tidy executable")

    # If paths are manually specified, use them
    if(CLANG_FORMAT_PATH AND CLANG_TIDY_PATH)
        if(EXISTS ${CLANG_FORMAT_PATH} AND EXISTS ${CLANG_TIDY_PATH})
            set(CLANG_FORMAT_EXEC ${CLANG_FORMAT_PATH} CACHE FILEPATH "Path to clang-format executable" FORCE)
            set(CLANG_TIDY_EXEC ${CLANG_TIDY_PATH} CACHE FILEPATH "Path to clang-tidy executable" FORCE)
            message(STATUS "Using manually specified clang tools:")
            message(STATUS "  clang-format: ${CLANG_FORMAT_EXEC}")
            message(STATUS "  clang-tidy: ${CLANG_TIDY_EXEC}")
            return()
        else()
            message(WARNING "Manually specified clang tool paths not found, falling back to automatic detection")
        endif()
    endif()

    # Try to find existing installations first
    find_program(CLANG_FORMAT_EXEC clang-format)
    find_program(CLANG_TIDY_EXEC clang-tidy)
    
    if(CLANG_FORMAT_EXEC AND CLANG_TIDY_EXEC)
        message(STATUS "Found existing clang tools:")
        message(STATUS "  clang-format: ${CLANG_FORMAT_EXEC}")
        message(STATUS "  clang-tidy: ${CLANG_TIDY_EXEC}")
        return()
    endif()

    # If auto-install is disabled, exit with error
    if(NOT AUTO_INSTALL_MISSING_TOOLS)
        message(WARNING "Clang tools not found and AUTO_INSTALL_MISSING_TOOLS is OFF. Please either:")
        message(WARNING "1. Install LLVM manually")
        message(WARNING "2. Add clang tools to your PATH")
        message(WARNING "3. Specify tool paths via -DCLANG_FORMAT_PATH=/path/to/clang-format -DCLANG_TIDY_PATH=/path/to/clang-tidy")
        message(WARNING "4. Enable automatic installation with -DAUTO_INSTALL_MISSING_TOOLS=ON")
        message(FATAL_ERROR "Missing required clang tools")
        return()
    endif()

    if(APPLE)
        # Check if Homebrew is installed
        find_program(BREW_EXECUTABLE brew)
        if(BREW_EXECUTABLE)
            # Get Homebrew prefix
            execute_process(
                COMMAND ${BREW_EXECUTABLE} --prefix llvm
                OUTPUT_VARIABLE LLVM_PREFIX
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )

            # Try to install LLVM if not present
            if(NOT EXISTS ${LLVM_PREFIX})
                message(STATUS "Installing LLVM via Homebrew...")
                execute_process(
                    COMMAND ${BREW_EXECUTABLE} install llvm
                    RESULT_VARIABLE BREW_RESULT
                )
                if(BREW_RESULT EQUAL 0)
                    # Add LLVM to PATH for this CMake session
                    set(ENV{PATH} "${LLVM_PREFIX}/bin:$ENV{PATH}")
                    set(CLANG_FORMAT_EXEC "${LLVM_PREFIX}/bin/clang-format" CACHE FILEPATH "Path to clang-format executable" FORCE)
                    set(CLANG_TIDY_EXEC "${LLVM_PREFIX}/bin/clang-tidy" CACHE FILEPATH "Path to clang-tidy executable" FORCE)
                    
                    if(EXISTS ${CLANG_FORMAT_EXEC} AND EXISTS ${CLANG_TIDY_EXEC})
                        message(STATUS "Successfully installed LLVM tools via Homebrew")
                        return()
                    endif()
                endif()
            endif()
        endif()

        message(WARNING "Could not install LLVM via Homebrew. Please either:")
        message(WARNING "1. Install LLVM manually: brew install llvm")
        message(WARNING "2. Add LLVM to your PATH")
        message(WARNING "3. Specify tool paths via -DCLANG_FORMAT_PATH=/path/to/clang-format -DCLANG_TIDY_PATH=/path/to/clang-tidy")

    elseif(WIN32)
        # Check for package managers and try to install LLVM
        find_program(CHOCO_EXECUTABLE choco)
        find_program(WINGET_EXECUTABLE winget)

        if(CHOCO_EXECUTABLE)
            message(STATUS "Attempting to install LLVM via Chocolatey...")
            execute_process(
                COMMAND ${CHOCO_EXECUTABLE} install llvm -y
                RESULT_VARIABLE CHOCO_RESULT
            )
            if(CHOCO_RESULT EQUAL 0)
                # Refresh PATH environment
                execute_process(COMMAND ${CMAKE_COMMAND} -E env PATH=${CMAKE_BINARY_DIR})
                find_program(CLANG_FORMAT_EXEC clang-format)
                find_program(CLANG_TIDY_EXEC clang-tidy)
                if(CLANG_FORMAT_EXEC AND CLANG_TIDY_EXEC)
                    message(STATUS "Successfully installed LLVM tools via Chocolatey")
                    return()
                endif()
            endif()
        elseif(WINGET_EXECUTABLE)
            message(STATUS "Attempting to install LLVM via winget...")
            execute_process(
                COMMAND ${WINGET_EXECUTABLE} install LLVM.LLVM
                RESULT_VARIABLE WINGET_RESULT
            )
            if(WINGET_RESULT EQUAL 0)
                # Refresh PATH environment
                execute_process(COMMAND ${CMAKE_COMMAND} -E env PATH=${CMAKE_BINARY_DIR})
                find_program(CLANG_FORMAT_EXEC clang-format)
                find_program(CLANG_TIDY_EXEC clang-tidy)
                if(CLANG_FORMAT_EXEC AND CLANG_TIDY_EXEC)
                    message(STATUS "Successfully installed LLVM tools via winget")
                    return()
                endif()
            endif()
        endif()

        message(WARNING "Could not install or find LLVM tools. Please either:")
        message(WARNING "1. Install LLVM manually from https://releases.llvm.org/")
        message(WARNING "2. Install via package manager: 'choco install llvm' or 'winget install LLVM.LLVM'")
        message(WARNING "3. Add LLVM to your PATH")
        message(WARNING "4. Specify tool paths via -DCLANG_FORMAT_PATH=/path/to/clang-format -DCLANG_TIDY_PATH=/path/to/clang-tidy")
    endif()

    message(FATAL_ERROR "Could not find or install clang tools. Please follow the instructions above.")
endfunction() 