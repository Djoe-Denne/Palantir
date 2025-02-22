# Function to find a program
function(find_program_or_warn VAR NAME)
    find_program(${VAR} NAMES ${NAME})
    if (NOT ${VAR})
        set(${VAR} "" PARENT_SCOPE)
    endif()
endfunction()

# Option to specify files to lint (empty means all files)
set(LINT_FILES "" CACHE STRING "Semicolon-separated list of files to lint. If empty, all files will be linted.")

# Check if clang-format and clang-tidy exist
find_program_or_warn(CLANG_FORMAT_EXEC clang-format)
find_program_or_warn(CLANG_TIDY_EXEC clang-tidy)

if (CLANG_FORMAT_EXEC AND CLANG_TIDY_EXEC)
    message(STATUS "✅ Clang tools found: ${CLANG_FORMAT_EXEC}, ${CLANG_TIDY_EXEC}")

    if (ALL_SOURCES)
        # Determine which files to lint
        set(FILES_TO_LINT ${ALL_SOURCES})
        if (LINT_FILES)
            set(FILES_TO_LINT ${LINT_FILES})
            message(STATUS "🔍 Linting specific files: ${LINT_FILES}")
        endif()

        # Add format target
        add_custom_target(format
            COMMAND ${CMAKE_COMMAND} -E echo "Running clang-format..."
            COMMAND ${CLANG_FORMAT_EXEC} -style=file -i ${FILES_TO_LINT}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Formatting source files"
            VERBATIM
        )

        # Add format-check target (verify formatting without changing files)
        add_custom_target(format-check
            COMMAND ${CMAKE_COMMAND} -E echo "Checking formatting..."
            COMMAND ${CLANG_FORMAT_EXEC} -style=file -output-replacements-xml ${SOURCE_FILES} | tee ${CMAKE_BINARY_DIR}/clang-format.xml | grep -c "<replacement " > /dev/null && exit 1 || exit 0
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} 
            COMMENT "Checking if sources are properly formatted"
            VERBATIM
        )

        # Export compile commands for clang-tidy
        set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Enable/Disable output of compile commands during generation." FORCE)

        # Create a list of include directories and compiler flags
        set(COMPILER_FLAGS
            -std=c++17
            -I${PROJECT_ROOT}/include
            -I${PROJECT_ROOT}/include/mode/debug
            -I${PROJECT_ROOT}/include/mode/release
        )
        if(WIN32)
            list(APPEND COMPILER_FLAGS -I${PROJECT_ROOT}/include/platform/windows)
        elseif(APPLE)
            list(APPEND COMPILER_FLAGS -I${PROJECT_ROOT}/include/platform/macos)
        endif()

        # Add lint target with compile_commands.json
        add_custom_target(lint
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/lint-reports
            COMMAND ${CLANG_TIDY_EXEC} 
                ${FILES_TO_LINT} 
                -p=${CMAKE_BINARY_DIR} 
                --quiet
                --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
                --format-style=file
                --fix
                --
                ${COMPILER_FLAGS}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running clang-tidy to fix issues"
            VERBATIM
        )

        # Add lint-check target (verify without fixing)
        add_custom_target(lint-check
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/lint-reports
            COMMAND ${CLANG_TIDY_EXEC} 
                ${FILES_TO_LINT} 
                -p=${CMAKE_BINARY_DIR} 
                --quiet
                --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
                --format-style=file
                --
                ${COMPILER_FLAGS}
                > ${CMAKE_BINARY_DIR}/lint-reports/report.txt
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running clang-tidy check"
            VERBATIM
        )

        # Add lint-analyze target to process the lint report
        add_custom_target(lint-analyze
            COMMAND ${CMAKE_COMMAND} 
                -DREPORT_FILE=${CMAKE_BINARY_DIR}/lint-reports/report.txt
                -DWARNING_THRESHOLD=5
                -P ${CMAKE_SOURCE_DIR}/cmake/analyze-lint.cmake
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Analyzing lint report"
            VERBATIM
        )

        # Add a combined target that runs both format and lint
        add_custom_target(fix-all
            COMMENT "Running all formatters and linters with fixes"
            VERBATIM
        )
        add_dependencies(fix-all format lint)

        # Add a combined check target
        add_custom_target(check-all
            COMMENT "Running all format and lint checks"
            VERBATIM
        )
        add_dependencies(check-all format-check lint-check)

    else()
        message(WARNING "⚠️ No source files found to format/lint.")
    endif()

else()
    message(WARNING "❌ Clang tools not found. Please wait for the automatic setup to complete or check the setup-clang-tools.cmake output.")

    # Create a placeholder target that prints an error message
    add_custom_target(clang-missing
        COMMAND ${CMAKE_COMMAND} -E echo "❌ Error: clang-format and/or clang-tidy not found."
        COMMAND ${CMAKE_COMMAND} -E echo "➡️  Please check the CMake configuration output for any setup errors."
        COMMAND ${CMAKE_COMMAND} -E false
    )
endif()
