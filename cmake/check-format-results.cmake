# Read the format check results
file(READ "${CMAKE_BINARY_DIR}/format-reports/format-check.xml" FORMAT_RESULTS)

# Check if there are any replacements in the XML
string(FIND "${FORMAT_RESULTS}" "<replacement " HAS_REPLACEMENTS)

if(NOT HAS_REPLACEMENTS EQUAL -1)
    message(FATAL_ERROR "❌ Formatting check failed - some files need formatting. Please run 'cmake --build . --target format' to fix.")
else()
    message(STATUS "✅ All files are properly formatted.")
endif() 
