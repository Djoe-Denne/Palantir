# Clang Tools Usage

This document describes how we use clang-format and clang-tidy in our project for code formatting and static analysis.

## Available Commands

### Code Formatting
- `cmake --build build --target format` - Format all source files in-place
- `cmake --build build --target format-check` - Check if files are properly formatted without modifying them

### Static Analysis
- `cmake --build build --target lint` - Run clang-tidy and fix issues automatically
- `cmake --build build --target lint-check` - Run clang-tidy in check-only mode
- `cmake --build build --target lint-analyze` - Analyze the lint report (fails if more than 5 warnings)

### Combined Commands
- `cmake --build build --target fix-all` - Run both formatting and linting with automatic fixes
- `cmake --build build --target check-all` - Run both formatting and linting checks without modifications

## Configuration Files
- `.clang-format` - Defines our code formatting style
- `.clang-tidy` - Defines our static analysis rules and checks

## Warning Threshold
The project enforces a maximum of 5 lint warnings. This threshold can be adjusted by modifying:
- `cmake/analyze-lint.cmake` - For local builds
- `cmake/format-lint.cmake` - In the lint-analyze target definition

## Clang Tools Guide

## Overview

The project uses two main clang tools for code quality:
- clang-format: Code formatting
- clang-tidy: Static analysis and linting

## Setup

The tools are automatically set up through `cmake/setup-clang-tools.cmake`. The setup:
1. Checks for existing installations
2. Attempts automatic installation if missing
3. Provides manual override options

### Manual Configuration

You can manually specify tool paths:
```bash
cmake -DCLANG_FORMAT_PATH=/path/to/clang-format -DCLANG_TIDY_PATH=/path/to/clang-tidy ..
```

## Clang Format

### Configuration

The formatting rules are defined in `.clang-format`:
```yaml
BasedOnStyle: Google
IndentWidth: 4
ColumnLimit: 120
# ... other style options ...
```

### Usage

Format all source files:
```bash
cmake --build . --target format
```

Check formatting without changes:
```bash
cmake --build . --target format-check
```

### Implementation

The format targets are defined in `cmake/format-lint.cmake`:
```cmake
add_custom_target(format
    COMMAND ${CLANG_FORMAT_EXEC} -style=file -i ${ALL_SOURCES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Formatting source files"
)
```

## Clang Tidy

### Configuration

Linting rules are defined in `.clang-tidy`:
```yaml
Checks: >
  -*,
  clang-analyzer-*,
  modernize-*,
  performance-*,
  readability-*,
  -clang-diagnostic-*

CheckOptions:
  - key: readability-identifier-naming.ClassCase
    value: CamelCase
  # ... other options ...
```

### Usage

Fix linting issues:
```bash
cmake --build . --target lint
```

Check without fixing:
```bash
cmake --build . --target lint-check
```

### Implementation

The lint targets use compile_commands.json:
```cmake
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_custom_target(lint
    COMMAND ${CLANG_TIDY_EXEC} 
        -p=${CMAKE_BINARY_DIR} 
        ${ALL_SOURCES} 
        --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
        --fix
)
```

## Combined Targets

### fix-all

Runs both formatting and linting with fixes:
```bash
cmake --build . --target fix-all
```

### check-all

Runs both format and lint checks:
```bash
cmake --build . --target check-all
```

## Troubleshooting

### Common Issues

1. **Missing Tools**
   - Error: "Clang tools not found"
   - Solution: Enable auto-install or install manually

2. **Compilation Database**
   - Error: "No compilation database found"
   - Solution: Ensure CMAKE_EXPORT_COMPILE_COMMANDS is ON

3. **Diagnostic Errors**
   - Issue: Unwanted diagnostic errors
   - Solution: Add to disabled checks in .clang-tidy:
     ```yaml
     -clang-diagnostic-*,
     -clang-diagnostic-error
     ```

### Manual Installation

#### Windows
```bash
choco install llvm
# or
winget install LLVM.LLVM
```

#### macOS
```bash
brew install llvm
``` 