# Function to find a program
function(find_program_or_warn VAR NAME)
    find_program(${VAR} NAMES ${NAME})
    if (NOT ${VAR})
        set(${VAR} "" PARENT_SCOPE)
    endif()
endfunction()

# Check if clang-format and clang-tidy exist
find_program_or_warn(CLANG_FORMAT_EXEC clang-format)
find_program_or_warn(CLANG_TIDY_EXEC clang-tidy)

if (CLANG_FORMAT_EXEC AND CLANG_TIDY_EXEC)
    message(STATUS "✅ Clang tools found: ${CLANG_FORMAT_EXEC}, ${CLANG_TIDY_EXEC}")

    # Collect all source files for formatting/linting
    file(GLOB_RECURSE SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_SOURCE_DIR}/src/*.mm"
    )

    if (SOURCE_FILES)
        # Add format target
        add_custom_target(format
            COMMAND ${CLANG_FORMAT_EXEC} -i ${SOURCE_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running clang-format"
        )

        # Add lint target with compile_commands.json
        add_custom_target(lint
            COMMAND ${CLANG_TIDY_EXEC} -p=${CMAKE_BINARY_DIR} ${SOURCE_FILES} ${CLANG_TIDY_IGNORED} --fix
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running clang-tidy"
        )
    else()
        message(WARNING "⚠️ No source files found to format/lint.")
    endif()

else()
    message(WARNING "❌ Clang tools not found. Please install Clang from: https://github.com/llvm/llvm-project/releases")
    message(WARNING "   Once installed, rerun CMake configuration (delete CMakeCache.txt or run cmake -B build again).")

    # Create a placeholder target that prints an error message
    add_custom_target(clang-missing
        COMMAND ${CMAKE_COMMAND} -E echo "❌ Error: clang-format and/or clang-tidy not found."
        COMMAND ${CMAKE_COMMAND} -E echo "➡️  Please install Clang from https://github.com/llvm/llvm-project/releases"
        COMMAND ${CMAKE_COMMAND} -E echo "🔄 After installing, rerun CMake configuration (delete CMakeCache.txt or run cmake -B build)."
        COMMAND ${CMAKE_COMMAND} -E false
    )
endif()
