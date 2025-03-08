# Script to analyze clang-tidy lint report
# Parameters:
# - REPORT_FILE: Path to the lint report file
# - WARNING_THRESHOLD: Maximum allowed number of warnings (default: 5)
# - GITHUB_REPORT: Path to the GitHub report file (optional)

if(NOT DEFINED REPORT_FILE)
    message(FATAL_ERROR "REPORT_FILE not defined")
endif()

if(NOT DEFINED WARNING_THRESHOLD)
    set(WARNING_THRESHOLD 5)
endif()

if(NOT EXISTS "${REPORT_FILE}")
    message(FATAL_ERROR "Report file not found: ${REPORT_FILE}")
endif()

# Read the report file
file(READ "${REPORT_FILE}" REPORT_CONTENT)

# Filter out warnings from CMakeLists.txt and GTest-related files
string(REGEX REPLACE "[^\n]*CMakeLists\\.txt:[^\n]*\n" "" FILTERED_CONTENT "${REPORT_CONTENT}")
string(REGEX REPLACE "[^\n]*googletest[^\n]*\n" "" FILTERED_CONTENT "${FILTERED_CONTENT}")
string(REGEX REPLACE "[^\n]*gtest[^\n]*\n" "" FILTERED_CONTENT "${FILTERED_CONTENT}")
string(REGEX REPLACE "[^\n]*gmock[^\n]*\n" "" FILTERED_CONTENT "${FILTERED_CONTENT}")

# Extract only warning lines for GitHub report
string(REGEX MATCHALL "[^\n]*warning:[^\n]*\n" WARNING_LINES "${FILTERED_CONTENT}")
string(REPLACE ";" "" WARNING_LINES "${WARNING_LINES}")


# Count warnings using string operations on filtered content
string(REGEX MATCHALL "warning:" WARNINGS "${FILTERED_CONTENT}")
list(LENGTH WARNINGS WARNING_COUNT)

message(STATUS "Found ${WARNING_COUNT} lint warnings")

# Check against threshold
if(WARNING_COUNT GREATER ${WARNING_THRESHOLD})
    message(FATAL_ERROR "Too many lint warnings: ${WARNING_COUNT} (threshold: ${WARNING_THRESHOLD})")
endif()

message(STATUS "Lint analysis passed: warnings within threshold") 

if(WARNING_COUNT GREATER 0)
    if(EXISTS "${CMAKE_BINARY_DIR}/build/lint-reports/github-report.txt")
        file(REMOVE "${CMAKE_BINARY_DIR}/build/lint-reports/github-report.txt")
    endif()

    set(REPORT_TEMPLATE "⚠️ Clang-Tidy Warnings Report:\n${WARNING_LINES}\n")
else()
    set(REPORT_TEMPLATE " 🟢 No lint warnings found")
endif()
file(WRITE "${CMAKE_BINARY_DIR}/build/lint-reports/github-report.txt" "${REPORT_TEMPLATE}")