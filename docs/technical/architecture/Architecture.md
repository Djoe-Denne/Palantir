# System Architecture

## Overview

The Interview Cheater application follows a modular, layered architecture that emphasizes separation of concerns, extensibility, and platform independence. The system is built using modern C++ design patterns and principles to ensure maintainability, testability, and robustness.

## Core Design Principles

### 1. Separation of Concerns
- Clear boundaries between system components
- Platform-specific code isolated in dedicated implementations
- Business logic separated from UI and configuration
- Implementation details hidden through PIMPL pattern

### 2. Thread Safety
- Singleton instances provide thread-safe access
- Synchronized key registration and signal processing
- Thread-safe event handling and command execution
- Protected shared resource access

### 3. Extensibility
- Plugin-based command system
- Platform-agnostic input abstraction
- Configurable keyboard shortcuts
- Modular window management

### 4. Configuration-Driven
- External configuration files
- Runtime behavior customization
- Default configurations provided
- Validation at load time

## System Components

### 1. Command System
- **ICommand Interface**: Base contract for all commands
- **CommandFactory**: Singleton factory for command management
  - PIMPL pattern implementation
  - Thread-safe command registration
  - Instance-based access
- **AutoCommandRegister**: Automatic command registration utility
- **Built-in Commands**:
  - ShowCommand: Window visibility control
  - StopCommand: Application termination
  - WindowScreenshotCommand: Window capture functionality
    - Platform-specific implementations
    - Automatic file naming and storage
    - Thread-safe file operations
    - GDI+ integration (Windows)
    - CoreGraphics integration (macOS)

### 2. Input System
- **Input Layer**
  - IInput Interface: Platform-agnostic input detection
  - ConfigurableInput: Keyboard shortcut implementation
  - KeyRegister: Singleton key registration service
  - KeyMapper: Key configuration mapping
  - KeyConfig: Configuration management

- **Signal Layer**
  - ISignal Interface: Signal processing contract
  - Signal: Generic signal implementation
  - SignalManager: Signal lifecycle management
  - SignalFactory: Signal creation service

### 3. Window System
- **WindowManager**: Singleton window management service
  - PIMPL pattern implementation
  - Window lifecycle management
  - Platform-specific window handling

### 4. Application Core
- **Application**: Base application functionality
  - Configuration management
  - Component initialization
  - Resource management

- **PlatformApplication**: Platform-specific implementation
  - Windows/macOS specific event loops
  - Native window management
  - System integration

## Implementation Patterns

### 1. PIMPL (Pointer to Implementation)
Used throughout the system to:
- Hide implementation details
- Reduce compilation dependencies
- Enable binary compatibility
- Facilitate platform-specific implementations

Example components using PIMPL:
- CommandFactory
- WindowManager
- KeyRegister
- ConfigurableInput

### 2. Singleton Pattern
Applied to key system services:
- CommandFactory: Command creation and management
- WindowManager: Window lifecycle and state
- KeyRegister: Key code registration and mapping
- SignalManager: Signal processing and management

### 3. Factory Pattern
Used for object creation and configuration:
- CommandFactory: Command instantiation
- InputFactory: Input handler creation
- SignalFactory: Signal creation and configuration
- WindowFactory: Window creation

## Platform-Specific Implementation

### Windows
- Low-level keyboard hook system
- Global event monitoring
- Win32 API integration
- Windows message loop
- GDI+ based window capture
  - HBITMAP creation and management
  - PNG encoding and compression
  - Memory management with RAII

### macOS
- Event monitor system
- Cocoa framework integration
- Global and local event handling
- Accessibility features
- CoreGraphics window capture
  - CGWindowListCreateImage usage
  - Window identification and focus detection
  - Image format conversion and storage

## Configuration System

### 1. Shortcut Configuration
- INI file format
- Command-to-key mapping
- Platform-specific modifiers
- Default configuration support

### 2. Input Configuration
- Key code mapping
- Platform-specific key registration
- Modifier key support
- Input validation

## Error Handling

### 1. Configuration Errors
- Validation at load time
- Default fallback mechanisms
- Descriptive error messages
- Configuration syntax checking

### 2. Runtime Errors
- Platform-specific error handling
- Graceful degradation
- Error logging and reporting
- Resource cleanup

## Best Practices

1. **Command Implementation**
   - Single responsibility principle
   - Clear command naming
   - Proper error handling
   - Stateless when possible
   - Platform-specific code isolation
   - Resource cleanup with RAII

2. **Resource Management**
   - RAII principles
   - Smart pointer usage
   - Proper cleanup in destructors
   - Exception safety
   - Automatic resource release
   - Memory leak prevention

3. **Thread Safety**
   - Synchronized access to shared resources
   - Thread-safe singleton access
   - Protected signal processing
   - Safe event handling

4. **Platform Independence**
   - Abstract interfaces
   - Platform-specific implementations
   - Configuration-driven behavior
   - Portable code practices

## Testing Architecture

### 1. Test Organization
- **Unit Tests**: Component-level testing
  - Command tests
  - Plugin tests
  - Platform-specific implementation tests
- **Mock Objects**: Test doubles for dependencies
  - MockApplication
  - MockWindow
  - MockWindowManager
- **Test Fixtures**: Common test setup and teardown
  - Platform-specific test environments
  - Resource cleanup
  - Test data management

### 2. Testing Patterns
- **Platform-Specific Testing**
  - Conditional compilation (#ifdef)
  - Platform-specific test suites
  - Environment setup and teardown
  - Resource management testing
- **Resource Management Tests**
  - Memory leak detection
  - Resource cleanup verification
  - Exception safety testing
- **Command Testing**
  - Command execution verification
  - Error handling scenarios
  - Platform-specific behavior testing

### 3. Test Infrastructure
- **Google Test Framework**
  - Test case organization
  - Assertion macros
  - Test fixtures
  - Test parameterization
- **Google Mock Framework**
  - Mock object creation
  - Behavior verification
  - Expectation setting
  - Method call tracking

### 4. Test Best Practices
- Isolated test cases
- Platform-specific test separation
- Comprehensive mock objects
- Resource cleanup verification
- Exception handling testing
- Thread safety verification
- Performance benchmarking 