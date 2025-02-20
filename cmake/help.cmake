function(setup_help_target)
    add_custom_target(helper
        COMMAND ${CMAKE_COMMAND} -E echo "Available targets:"
        COMMAND ${CMAKE_COMMAND} -E echo "  build        - Build the project"
        COMMAND ${CMAKE_COMMAND} -E echo "  clean        - Clean build files"
        COMMAND ${CMAKE_COMMAND} -E echo ""
        COMMAND ${CMAKE_COMMAND} -E echo "Code quality targets:"
        COMMAND ${CMAKE_COMMAND} -E echo "  format       - Format source files"
        COMMAND ${CMAKE_COMMAND} -E echo "  format-check - Check if sources are properly formatted"
        COMMAND ${CMAKE_COMMAND} -E echo "  lint         - Run clang-tidy and fix issues"
        COMMAND ${CMAKE_COMMAND} -E echo "  lint-check   - Run clang-tidy check without fixing"
        COMMAND ${CMAKE_COMMAND} -E echo "  fix-all      - Run all formatters and linters with fixes"
        COMMAND ${CMAKE_COMMAND} -E echo "  check-all    - Run all format and lint checks"
        COMMAND ${CMAKE_COMMAND} -E echo ""
        COMMAND ${CMAKE_COMMAND} -E echo "Configuration options:"
        COMMAND ${CMAKE_COMMAND} -E echo "  -DAUTO_INSTALL_MISSING_TOOLS=OFF  - Disable automatic installation of clang tools"
        COMMAND ${CMAKE_COMMAND} -E echo "  -DCLANG_FORMAT_PATH=/path/to/clang-format  - Manually specify clang-format path"
        COMMAND ${CMAKE_COMMAND} -E echo "  -DCLANG_TIDY_PATH=/path/to/clang-tidy      - Manually specify clang-tidy path"
        VERBATIM
    )
endfunction() 