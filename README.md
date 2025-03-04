# Palantir Project

## Overview
The Palantir project is a cross-platform application designed to create an overlay window that can be controlled via keyboard shortcuts. It utilizes a modular architecture with interfaces and command patterns to manage window operations and input signals. The application includes AI integration through Sauron SDK for assisting with coding interviews.

## Features
- **Overlay Window**: Displays a floating window with interview notes
- **Keyboard Input Handling**: Listens for specific key combinations to control the overlay window
- **Command Pattern**: Implements commands for showing and stopping the overlay window
- **Signal Management**: Monitors keyboard signals and executes corresponding commands
- **Screen Sharing Protection**: Window is hidden from screen recording/sharing
- **Cross-Space Support**: Window can be visible across all spaces
- **AI Integration**: Connects to Sauron SDK for AI-powered assistance
- **Content Management**: Manages and displays content in the overlay window
- **Screenshot Capture**: Takes screenshots of the current window for AI analysis

## File Structure
```
palantir
├── palantir-core/           # Core library
├── plugins/                 # Plugin modules
│   ├── commands/            # Command implementations
├── palentir-webapp/         # Web application frontend
├── docs/                    # Documentation
│   ├── technical/           # Technical documentation
│   ├── user/                # User documentation
├── cmake/                   # CMake configuration files
├── resources/               # Resource files
├── .clang-format            # Clang format configuration
├── .clang-tidy              # Clang tidy configuration
├── .clangchecklist          # Clang checklist configuration
├── CMakeLists.txt           # Main CMake configuration
└── README.md                # Project documentation
```

## Prerequisites
- CMake 3.10 or higher
- Clang tools (automatically installed via Homebrew on macOS)
- Xcode Command Line Tools (for macOS)
- Visual Studio 2019 or higher with C++ workload (for Windows)

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
2. Use keyboard shortcuts to control the application:
   - Press `Ctrl+Num 1` to toggle the overlay window
   - Press `Ctrl+Num 2` to take a screenshot of the current window
   - Press `Ctrl+Num 3` to toggle window transparency
   - Press `Ctrl+Num 4` to send an implementation request to Sauron
   - Press `Ctrl+Num 5` to send a fix errors request to Sauron
   - Press `Ctrl+Num 6` to send a validate with tests request to Sauron
   - Press `Ctrl+Num 7` to send a fix test failures request to Sauron
   - Press `Ctrl+Num 8` to send a handle TODOs request to Sauron
   - Press `Ctrl+Num 9` to clear the screenshot folder
   - Press `Win+/` to stop the application
3. The window will stay on top and be visible across all spaces
4. The window is automatically hidden from screen sharing/recording
5. Window can be moved by dragging anywhere on its surface

## Platform Support
- Windows: Fully supported with native implementation
- macOS: Support planned for future releases

## License
This project is licensed under the MIT License. See the LICENSE file for more details.