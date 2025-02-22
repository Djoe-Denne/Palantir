# CMake Configuration

This document describes how we use CMake in our project for building and development workflows.

## Build Types

- `Release` - Optimized build for distribution
- `Debug` - Build with debug symbols and minimal optimization

## Common Commands

### Basic Build
```bash
# Configure the build
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build build --config Release
```

### Development Workflow
```bash
# Configure with compile commands export (needed for clang-tidy)
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Build in debug mode
cmake --build build --config Debug

# Selective linting of specific files
cmake -B build -DLINT_FILES="path/to/file1.cpp;path/to/file2.hpp"
```

## Custom Targets

Our project defines several custom targets for code quality:

### Code Quality
- `format-check` - Check code formatting
- `lint-check` - Run static analysis
- `lint-analyze` - Process lint report
- `check-all` - Run all checks

### Code Fixes
- `format` - Apply code formatting
- `lint` - Apply static analysis fixes
- `fix-all` - Apply all available fixes

### Selective Quality Checks
All quality targets support the `LINT_FILES` option:
```bash
# Format specific files
cmake -DLINT_FILES="file1.cpp;file2.cpp" -B build
cmake --build build --target format

# Lint specific files
cmake --build build --target lint
```

## Platform-Specific Settings

The build system automatically configures:
- Include paths for Windows/macOS specific code
- Compiler flags based on the platform
- Output paths for executables and app bundles

### Platform Sources
Platform-specific sources are defined in their respective CMake files:

#### Windows (`windows.cmake`)
```cmake
set(WINDOWS_SOURCES
    ${PROJECT_ROOT}/src/platform/windows/input/configurable_input.cpp
    # ... other Windows sources
)

set(ALL_SOURCES
    ${ALL_SOURCES}
    ${WINDOWS_SOURCES}
    ${WINDOWS_HEADERS}
)
```

#### macOS (`macos.cmake`)
```cmake
set(MACOS_SOURCES
    ${PROJECT_ROOT}/src/platform/macos/input/configurable_input.mm
    # ... other macOS sources
)

set(ALL_SOURCES
    ${ALL_SOURCES}
    ${MACOS_SOURCES}
    ${MACOS_HEADERS}
)
```

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

The project uses a specific pattern for handling source file variables:

1. Base Sources (in `common-source.cmake`):
```cmake
set(ALL_SOURCES
    ${CORE_SOURCES}
    ${COMMAND_SOURCES}
    ${SIGNAL_SOURCES}
    ${WINDOW_SOURCES}
    ${INPUT_SOURCES}
    ${PLATFORM_COMMON_SOURCES}
)
```

2. Platform-Specific Sources:
```cmake
# Platform sources are added directly to ALL_SOURCES
set(ALL_SOURCES
    ${ALL_SOURCES}
    ${PLATFORM_SOURCES}
    ${PLATFORM_HEADERS}
)
```

### 3. Platform-Specific Configuration

Platform configurations are included early to ensure proper source file collection:

```cmake
include(platform/windows)
include(platform/macos)

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${ALL_SOURCES})
    setup_windows_platform()
elseif(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${ALL_SOURCES})
    setup_macos_platform()
endif()
```

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

set(ALL_SOURCES
    ${ALL_SOURCES}
    ${YOUR_FEATURE_SOURCES}
)
```

### Platform-Specific Files

Add platform-specific files to the appropriate platform CMake file:

```cmake
# In windows.cmake or macos.cmake
set(PLATFORM_SOURCES
    ${PLATFORM_SOURCES}
    ${PROJECT_ROOT}/src/platform/<platform>/your_feature.cpp
)
```

## Code Quality Tools

### Clang Format

Format your code using:
```bash
# Format all files
cmake --build . --target format

# Format specific files
cmake -DLINT_FILES="file1.cpp;file2.cpp" -B build
cmake --build build --target format
```

Configuration: `.clang-format` file in project root

### Clang Tidy

Lint your code using:
```bash
# Lint all files
cmake --build . --target lint

# Lint specific files
cmake -DLINT_FILES="file1.cpp;file2.cpp" -B build
cmake --build build --target lint
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