function(setup_clang_tools)
    # Check if we're on macOS
    if(APPLE)
        # Check if Homebrew is installed
        find_program(BREW_EXECUTABLE brew)
        if(NOT BREW_EXECUTABLE)
            message(FATAL_ERROR "Homebrew is not installed. Please install it first: https://brew.sh")
        endif()

        # Get Homebrew prefix
        execute_process(
            COMMAND ${BREW_EXECUTABLE} --prefix llvm
            OUTPUT_VARIABLE LLVM_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        # Check if LLVM is installed
        if(NOT EXISTS ${LLVM_PREFIX})
            message(STATUS "Installing LLVM via Homebrew...")
            execute_process(
                COMMAND ${BREW_EXECUTABLE} install llvm
                RESULT_VARIABLE BREW_RESULT
            )
            if(NOT BREW_RESULT EQUAL 0)
                message(FATAL_ERROR "Failed to install LLVM via Homebrew")
            endif()
        endif()

        # Add LLVM to PATH for this CMake session
        set(ENV{PATH} "${LLVM_PREFIX}/bin:$ENV{PATH}")
        
        # Set the CLANG_FORMAT_EXEC and CLANG_TIDY_EXEC variables directly to the Homebrew paths
        set(CLANG_FORMAT_EXEC "${LLVM_PREFIX}/bin/clang-format" CACHE FILEPATH "Path to clang-format executable" FORCE)
        set(CLANG_TIDY_EXEC "${LLVM_PREFIX}/bin/clang-tidy" CACHE FILEPATH "Path to clang-tidy executable" FORCE)
        
        # Verify tools are available
        if(NOT EXISTS ${CLANG_FORMAT_EXEC} OR NOT EXISTS ${CLANG_TIDY_EXEC})
            message(FATAL_ERROR "LLVM tools not found in ${LLVM_PREFIX}/bin. Please ensure LLVM is properly installed via Homebrew.")
        endif()
        
        message(STATUS "Using clang tools from Homebrew LLVM installation:")
        message(STATUS "  clang-format: ${CLANG_FORMAT_EXEC}")
        message(STATUS "  clang-tidy: ${CLANG_TIDY_EXEC}")
    endif()
endfunction() 