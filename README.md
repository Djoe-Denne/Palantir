# Interview Cheater Project

## Overview
The Interview Cheater project is a Windows application designed to create an overlay window that can be controlled via keyboard shortcuts. It utilizes a modular architecture with interfaces and command patterns to manage window operations and input signals.

## Features
- **Overlay Window**: Displays a red square overlay on the screen.
- **Keyboard Input Handling**: Listens for specific key combinations to control the overlay window.
- **Command Pattern**: Implements commands for showing and stopping the overlay window.
- **Signal Management**: Monitors keyboard signals and executes corresponding commands.

## File Structure
```
interview-cheater
├── src
│   ├── main.cpp                # Entry point of the application
│   ├── WindowManager.hpp       # Header for WindowManager class
│   ├── WindowManager.cpp       # Implementation of WindowManager class
│   ├── IInput.hpp              # Interface for input handling
│   ├── Input.hpp        # Header for Input class
│   ├── Input.cpp        # Implementation of Input class
│   ├── IWindow.hpp             # Interface for window operations
│   ├── OverlayWindow.hpp # Header for OverlayWindow class
│   ├── OverlayWindow.cpp # Implementation of OverlayWindow class
│   ├── ICommand.hpp             # Interface for command execution
│   ├── ShowCommand.hpp          # Header for ShowCommand class
│   ├── ShowCommand.cpp          # Implementation of ShowCommand class
│   ├── StopCommand.hpp          # Header for StopCommand class
│   ├── StopCommand.cpp          # Implementation of StopCommand class
│   ├── ISignal.hpp              # Interface for signal checking
│   ├── CtrlF1Signal.hpp         # Header for CtrlF1Signal class
│   ├── CtrlF1Signal.cpp         # Implementation of CtrlF1Signal class
│   ├── SignalManager.hpp        # Header for SignalManager class
│   ├── SignalManager.cpp        # Implementation of SignalManager class
├── CMakeLists.txt              # CMake configuration file
└── README.md                   # Project documentation
```

## Build Instructions
1. Ensure you have CMake installed on your system.
2. Open a terminal and navigate to the project directory.
3. Create a build directory:
   ```
   mkdir build
   cd build
   ```
4. Run CMake to configure the project:
   ```
   cmake ..
   ```
5. Build the project:
   ```
   cmake --build .
   ```

## Usage
- Run the application to display the overlay window.
- Press `Ctrl + F1` to stop the overlay window.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.