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
    ${PROJECT_ROOT}/palantir-core/src/platform/windows/input/configurable_input.cpp
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
    ${PROJECT_ROOT}/palantir-core/src/platform/macos/input/configurable_input.mm
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
├── help.cmake               # Help target configuration
├── setup-clang-tools.cmake  # Clang tools setup
├── install-palantir-deps.cmake # Dependencies installation
└── platform/
    ├── windows.cmake        # Windows-specific configuration
    └── macos.cmake          # macOS-specific configuration

palantir-core/               # Core library
├── include/                 # Header files
│   ├── command/             # Command system headers
│   ├── input/               # Input system headers
│   ├── window/              # Window system headers
│   ├── client/              # Client system headers
│   └── ...
└── src/                     # Source files
    ├── command/             # Command system implementation
    ├── input/               # Input system implementation
    ├── window/              # Window system implementation
    ├── client/              # Client system implementation
    └── ...

plugins/                     # Plugin modules
├── commands/                # Command plugin
│   ├── include/             # Command plugin headers
│   └── src/                 # Command plugin implementation
└── ...

palentir-webapp/            # Web application frontend
```

## Main Components

### 1. Source Organization

Sources are organized into logical groups in `common-source.cmake`:

```cmake
# Headers (automatically found via GLOB)
file(GLOB_RECURSE PROJECT_HEADERS
    "${PROJECT_ROOT}/palantir-core/include/*.hpp"
    "${PROJECT_ROOT}/palantir-core/include/*.h"
)

# Source groups
set(CORE_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/main.cpp
)

set(COMMAND_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/command/command_factory.cpp
    # ... other command sources
)

# ... other source groups
```

### 2. Build Configuration

The main CMakeLists.txt includes platform configurations and then adds the core library and plugins:

```cmake
include(platform/windows)
include(platform/macos)

# Add the core library
add_subdirectory(palantir-core)

# Add plugins
add_subdirectory(plugins/commands)
```

### 3. Dependencies Management

The project uses FetchContent to manage dependencies:

```cmake
include(FetchContent)

# Sauron SDK
FetchContent_Declare(
    SAURON_SDK
    GIT_REPOSITORY https://github.com/Djoe-Denne/Sauron-sdk.git
    GIT_BRANCH master
)
FetchContent_MakeAvailable(SAURON_SDK)

# nlohmann_json
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)
```

### 4. Plugin System

Plugins are built as shared libraries and loaded dynamically:

```cmake
add_library(commands-plugin SHARED
    ${COMMANDS_PLUGIN_SOURCES}
    ${COMMANDS_PLUGIN_HEADERS}
)

target_link_libraries(commands-plugin PRIVATE palantir-core)
```

## Adding New Source Files

### Method 1: Adding to Existing Groups

Add your source files to the appropriate group in `common-source.cmake`:

```cmake
set(COMMAND_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/command/your_new_command.cpp
)
```

### Method 2: Creating New Groups

For new feature groups, add a new source group:

```cmake
set(YOUR_FEATURE_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/your_feature/feature_impl.cpp
)

set(ALL_SOURCES
    ${ALL_SOURCES}
    ${YOUR_FEATURE_SOURCES}
)
```

### Platform-Specific Files

Add platform-specific files to the appropriate platform CMake file:

```cmake
set(WINDOWS_SOURCES
    ${WINDOWS_SOURCES}
    ${PROJECT_ROOT}/palantir-core/src/platform/windows/your_feature/feature_impl.cpp
)
```

### Adding New Plugins

To add a new plugin:

1. Create a new directory in the `plugins` directory
2. Create a CMakeLists.txt file in the new directory
3. Define the plugin sources and headers
4. Add the plugin to the main CMakeLists.txt

```cmake
# In plugins/your_plugin/CMakeLists.txt
set(YOUR_PLUGIN_SOURCES
    ${PROJECT_ROOT}/plugins/your_plugin/src/your_plugin.cpp
)

set(YOUR_PLUGIN_HEADERS
    ${PROJECT_ROOT}/plugins/your_plugin/include/your_plugin.hpp
)

add_library(your-plugin SHARED
    ${YOUR_PLUGIN_SOURCES}
    ${YOUR_PLUGIN_HEADERS}
)

target_link_libraries(your-plugin PRIVATE palantir-core)
```

## Sauron SDK Integration

The project integrates with the Sauron SDK for AI-powered assistance:

```cmake
# In cmake/install-palantir-deps.cmake
function(install_palantir_deps)
    message(STATUS "Checking or installing palantir dependencies")
    find_package(Sauron-sdk QUIET)
    
    if((NOT SAURON_SDK_FOUND OR NOT TARGET sauron_sdk::curl) AND MAGIC_DEPS_INSTALL)
        # Define the repository URL
        if(GITHUB_TOKEN)
            set(GIT_REPOSITORY https://${GITHUB_TOKEN}@github.com/Djoe-Denne/Sauron-sdk.git)
        else()
            set(GIT_REPOSITORY https://github.com/Djoe-Denne/Sauron-sdk.git)
        endif()

        message(STATUS "System Sauron SDK not found, building from source...")
        # Find or install Sauron SDK
        include(FetchContent)
        FetchContent_Declare(
            SAURON_SDK
            GIT_REPOSITORY ${GIT_REPOSITORY}
            GIT_BRANCH master
        )
        FetchContent_MakeAvailable(SAURON_SDK) 
    endif()
    
    if(NOT SAURON_SDK_FOUND)
        message(FATAL_ERROR "Sauron SDK not found")
    endif()
    
    message(STATUS "Found system Sauron SDK: ${SAURON_SDK_LIBRARIES}")
endfunction()
```

## Content Management System

The project includes a content management system for displaying and managing content in the overlay window:

```cmake
# In palantir-core/CMakeLists.txt
set(CONTENT_MANAGER_SOURCES
    ${PROJECT_ROOT}/palantir-core/src/window/component/content_manager.cpp
)

set(CONTENT_MANAGER_HEADERS
    ${PROJECT_ROOT}/palantir-core/include/window/component/content_manager.hpp
    ${PROJECT_ROOT}/palantir-core/include/window/component/content_manager_impl.hpp
    ${PROJECT_ROOT}/palantir-core/include/window/component/icontent_manager.hpp
)

set(ALL_SOURCES
    ${ALL_SOURCES}
    ${CONTENT_MANAGER_SOURCES}
    ${CONTENT_MANAGER_HEADERS}
)
```

## Building for Different Platforms

### Windows
```bash
# Configure for Windows
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release
```

### macOS (Future Support)
```bash
# Configure for macOS
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release
```

## Testing

The project includes a testing framework:

```cmake
# Enable testing
enable_testing()

# Add test subdirectory
add_subdirectory(tests)
```

Tests can be run using:

```bash
# Run all tests
ctest -C Debug

# Run specific tests
ctest -C Debug -R "command_tests"
``` 