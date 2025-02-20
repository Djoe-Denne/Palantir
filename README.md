# Interview Cheater Project

## Overview
The Interview Cheater project is a macOS application designed to create an overlay window that can be controlled via keyboard shortcuts. It utilizes a modular architecture with interfaces and command patterns to manage window operations and input signals.

## Features
- **Overlay Window**: Displays a floating window with interview notes
- **Keyboard Input Handling**: Listens for specific key combinations to control the overlay window
- **Command Pattern**: Implements commands for showing and stopping the overlay window
- **Signal Management**: Monitors keyboard signals and executes corresponding commands
- **Screen Sharing Protection**: Window is hidden from screen recording/sharing
- **Cross-Space Support**: Window can be visible across all spaces

## File Structure
```
interview-cheater
├── src/                     # Source files
├── include/                 # Header files
├── cmake/                   # CMake configuration files
├── resources/               # Resource files
├── .clang-format           # Clang format configuration
├── .clang-tidy             # Clang tidy configuration
├── .clangchecklist         # Clang checklist configuration
├── CMakeLists.txt          # Main CMake configuration
└── README.md               # Project documentation
```

## Prerequisites
- CMake 3.10 or higher
- Clang tools (automatically installed via Homebrew on macOS)
- Xcode Command Line Tools (for macOS)

## Build Instructions

### Debug Build
```bash
# Configure the project in Debug mode
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug

# Build the project
cmake --build build/
```

### Release Build
```bash
# Configure the project in Release mode
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build build/
```

## Code Quality Tools

The project includes several code quality targets that can be run using CMake:

### Format Code
```bash
# Format all source files
cmake --build build --target format

# Check formatting without making changes
cmake --build build --target format-check
```

### Static Analysis
```bash
# Run clang-tidy and fix issues
cmake --build build --target lint

# Run clang-tidy check without fixing
cmake --build build --target lint-check
```

### Combined Targets
```bash
# Run both format and lint with fixes
cmake --build build --target fix-all

# Run both format and lint checks without fixing
cmake --build build --target check-all
```

## Usage
1. Build and run the application
2. Press `Command + /` to toggle the overlay window
3. The window will stay on top and be visible across all spaces
4. The window is automatically hidden from screen sharing/recording
5. Window can be moved by dragging anywhere on its surface

## Platform Support
- macOS: Fully supported with native Cocoa implementation
- Windows: Support planned for future releases

## License
This project is licensed under the MIT License. See the LICENSE file for more details.