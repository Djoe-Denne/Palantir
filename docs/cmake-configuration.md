# CMake Configuration Guide

## Project Structure

The project uses a modular CMake configuration split across multiple files for better organization:

```
cmake/
├── common-source.cmake      # Common source files and variables
├── format-lint.cmake        # Clang format and tidy configuration
├── help.cmake              # Help target configuration
├── setup-clang-tools.cmake # Clang tools setup
└── platform/
    ├── windows.cmake       # Windows-specific configuration
    └── macos.cmake        # macOS-specific configuration
```

## Main Components

### 1. Source Organization

Sources are organized into logical groups in `common-source.cmake`:

```cmake
# Headers (automatically found via GLOB)
file(GLOB_RECURSE PROJECT_HEADERS
    "${PROJECT_ROOT}/include/*.hpp"
    "${PROJECT_ROOT}/include/*.h"
)

# Source groups
set(CORE_SOURCES ...)
set(COMMAND_SOURCES ...)
set(SIGNAL_SOURCES ...)
set(WINDOW_SOURCES ...)
set(INPUT_SOURCES ...)
set(PLATFORM_COMMON_SOURCES ...)
```

### 2. Variable Scope Handling

The project uses a specific pattern for handling source file variables across different scopes:

1. Base Sources (in `common-source.cmake`):
```cmake
# Initialize ALL_SOURCES at top level (no PARENT_SCOPE needed)
set(ALL_SOURCES
    ${CORE_SOURCES}
    ${COMMAND_SOURCES}
    ${SIGNAL_SOURCES}
    ${WINDOW_SOURCES}
    ${INPUT_SOURCES}
    ${PLATFORM_COMMON_SOURCES}
    ${PROJECT_HEADERS}
)
```

2. Platform-Specific Sources (in platform/*.cmake):
```cmake
function(setup_platform)
    # Define platform sources
    set(PLATFORM_SOURCES ...)
    file(GLOB_RECURSE PLATFORM_HEADERS ...)

    # Append to ALL_SOURCES in parent scope
    set(ALL_SOURCES
        ${ALL_SOURCES}        # Include existing sources
        ${PLATFORM_SOURCES}   # Add platform sources
        ${PLATFORM_HEADERS}   # Add platform headers
        PARENT_SCOPE         # Required inside functions
    )
endfunction()
```

This approach ensures:
- Common sources are initialized at the top level
- Platform-specific sources are properly appended
- Variable scoping is handled correctly in functions
- All sources are available for both building and tools

### 3. Platform-Specific Configuration

- Windows configuration (`windows.cmake`):
  - Platform-specific sources and headers
  - Windows-specific compile definitions
  - Required Windows libraries (gdi32, dwmapi, user32)

- macOS configuration (`macos.cmake`):
  - Platform-specific sources and headers (.mm files)
  - macOS-specific compile options
  - Required macOS frameworks (Cocoa, Carbon)

## Adding New Source Files

### Method 1: Adding to Existing Groups

Add your source files to the appropriate group in `common-source.cmake`:

```cmake
set(COMMAND_SOURCES
    ${PROJECT_ROOT}/src/command/your_new_command.cpp
)
```

### Method 2: Creating New Groups

For new feature groups, add a new source group:

```cmake
set(YOUR_FEATURE_SOURCES
    ${PROJECT_ROOT}/src/your_feature/feature_impl.cpp
)
```

Then add it to `ALL_SOURCES`:

```cmake
set(ALL_SOURCES
    ${CORE_SOURCES}
    ${YOUR_FEATURE_SOURCES}
    # ... other sources ...
    ${PROJECT_HEADERS}
)
```

### Headers

Headers are automatically found using GLOB:
- Project headers: `include/*.hpp` and `include/*.h`
- Platform headers: `include/platform/<platform>/*.hpp` and `*.h`

## Code Quality Tools

### Clang Format

Format your code using:
```bash
cmake --build . --target format      # Apply formatting
cmake --build . --target format-check # Check formatting only
```

Configuration: `.clang-format` file in project root

### Clang Tidy

Lint your code using:
```bash
cmake --build . --target lint       # Fix issues
cmake --build . --target lint-check # Check only
```

Configuration: `.clang-tidy` file in project root

### Combined Targets

- `fix-all`: Run both format and lint with fixes
- `check-all`: Run both format and lint checks

## Build Configuration

### Debug vs Release

Debug configuration:
```cmake
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COMMON_INCLUDES
        ${PROJECT_ROOT}/include/mode/debug
    )
    add_definitions(-DDEBUG)
```

Release uses `include/mode/release` instead.

## Helper Targets

Run `cmake --build . --target helper` for a list of available targets and their descriptions. 