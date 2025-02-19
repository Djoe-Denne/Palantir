# Path to .clangchecklist
set(CLANG_CHECKLIST_FILE "${CMAKE_SOURCE_DIR}/.clangchecklist")

# Default to enabling no checks
set(CLANG_TIDY_CHECKS "")

# Read the .clangchecklist file if it exists
if(EXISTS "${CLANG_CHECKLIST_FILE}")
    file(STRINGS "${CLANG_CHECKLIST_FILE}" CLANG_CHECKS_RAW)

    # Convert list into -checks=<enabled_checks> format
    foreach(CHECK ${CLANG_CHECKS_RAW})
        if (NOT CHECK MATCHES "^#") # Ignore comment lines
            list(APPEND CLANG_TIDY_CHECKS_LIST "${CHECK}")
        endif()
    endforeach()

    # Join checks with commas (format required by Clang-Tidy)
    list(JOIN CLANG_TIDY_CHECKS_LIST "," CLANG_TIDY_CHECKS)
endif()
# Apply Clang-Tidy settings
set(CLANG_TIDY_IGNORED --checks=${CLANG_TIDY_CHECKS})
