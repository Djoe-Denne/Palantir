# Script to analyze clang-tidy lint report
# Parameters:
# - REPORT_FILE: Path to the lint report file
# - WARNING_THRESHOLD: Maximum allowed number of warnings (default: 5)

if(NOT DEFINED REPORT_FILE)
    message(FATAL_ERROR "REPORT_FILE not defined")
endif()

if(NOT DEFINED WARNING_THRESHOLD)
    set(WARNING_THRESHOLD 2)
endif()

if(NOT EXISTS "${REPORT_FILE}")
    message(FATAL_ERROR "Report file not found: ${REPORT_FILE}")
endif()

# Read the report file
file(READ "${REPORT_FILE}" REPORT_CONTENT)

# Count warnings using string operations
string(REGEX MATCHALL "warning:" WARNINGS "${REPORT_CONTENT}")
list(LENGTH WARNINGS WARNING_COUNT)

message(STATUS "Found ${WARNING_COUNT} lint warnings")

# Check against threshold
if(WARNING_COUNT GREATER ${WARNING_THRESHOLD})
    message(FATAL_ERROR "Too many lint warnings: ${WARNING_COUNT} (threshold: ${WARNING_THRESHOLD})")
endif()

message(STATUS "Lint analysis passed: warnings within threshold") 