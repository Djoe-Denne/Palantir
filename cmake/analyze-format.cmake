# Read the clang-format XML output file
file(READ "${CMAKE_BINARY_DIR}/build/format-reports/clang-format.xml" FORMAT_OUTPUT)

# Initialize variables
set(HAS_FORMATTING_ISSUES FALSE)
set(FILES_WITH_ISSUES "")

# Look for replacement tags in the XML output
string(REGEX MATCHALL "<replacement [^>]*>" REPLACEMENTS "${FORMAT_OUTPUT}")

if(REPLACEMENTS)
    set(HAS_FORMATTING_ISSUES TRUE)
    
    # Extract filenames from the XML output
    string(REGEX MATCHALL ".*<replacement offset=\"[0-9]+\" length=\"[0-9]+\">[^<]*</replacement>.*" FULL_REPLACEMENTS "${FORMAT_OUTPUT}")
    string(REGEX MATCHALL "<file>([^<]+)</file>" FILE_MATCHES "${FORMAT_OUTPUT}")
    
    # Create a list of unique files that need formatting
    foreach(FILE_MATCH ${FILE_MATCHES})
        string(REGEX REPLACE "<file>([^<]+)</file>" "\\1" FILENAME "${FILE_MATCH}")
        list(APPEND FILES_WITH_ISSUES "${FILENAME}")
    endforeach()
    
    list(REMOVE_DUPLICATES FILES_WITH_ISSUES)
    
    # Print the error message
    message(FATAL_ERROR 
        "❌ Formatting issues found in the following files:\n"
        "${FILES_WITH_ISSUES}\n"
        "Run 'cmake --build . --target format' to fix formatting issues."
    )
else()
    message(STATUS "✅ All files are properly formatted.")
endif() 