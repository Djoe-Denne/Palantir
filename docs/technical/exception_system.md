# Exception System

## Overview

Palantir implements a robust exception handling system that provides:

1. **Hierarchical Exception Classes**: A well-structured inheritance hierarchy for specific error types
2. **Stack Trace Support**: Enhanced exceptions with stack trace information when compiler support is available
3. **Platform-Specific Implementations**: Fallback implementations for platforms without native stack trace support
4. **Consistent Error Handling**: Uniform approach to error reporting across the application

## Exception Hierarchy

The exception system is built on a base class that inherits from `std::runtime_error`:

```cpp
namespace palantir::exception {

class BaseException : public std::runtime_error {
public:
    explicit BaseException(const std::string& message) : std::runtime_error(message) {}
    virtual ~BaseException() = default;
};

}  // namespace palantir::exception
```

### Domain-Specific Exceptions

The system defines various domain-specific exceptions for different subsystems:

#### Application Exceptions
- `NavigationException`: Navigation failures in the WebView
- `JavaScriptExecutionException`: JavaScript execution failures
- `MessageSendException`: Failures when sending messages to JavaScript
- `WindowOperationException`: Window creation or manipulation failures
- `KeyboardHookException`: Keyboard hook installation failures

#### Configuration Exceptions
- `ConfigFileException`: Configuration file operations failures
- `ContentManagerException`: Content management errors
- `ResourceLoadingException`: Resource loading failures

#### Input Exceptions
- `InputFactoryException`: Input factory initialization failures
- `ShortcutConfigurationException`: Shortcut parsing and handling errors

#### Plugin Exceptions
- `PluginInitializationException`: Plugin initialization failures
- `UnknownCommandException`: Unknown command encountered
- `UIComponentNotFoundException`: UI component not found

## Traceable Exceptions

The system provides a `TraceableException` template that enhances any existing exception with stack trace capabilities:

```cpp
template <typename E>
    requires std::is_base_of_v<std::exception, E>
class TraceableException : public E, public TraceableBaseException {
public:
    ~TraceableException() override = default;
    explicit TraceableException(const std::string &what) : E(what) { ... }
    std::string getStackTraceString() const override { ... }
    // Platform-specific implementations
};
```

### Platform Support

The `TraceableException` template provides:

1. **C++23 Standard Support**: Uses `std::stacktrace` when available (modern compilers)
2. **Windows Implementation**: Uses Windows-specific APIs (DbgHelp)
3. **Linux/macOS Implementation**: Uses `backtrace` and `backtrace_symbols`
4. **Fallback Implementation**: Provides graceful degradation on unsupported platforms

## Usage

The exception system can be used in two ways:

### Direct Exception Throwing

```cpp
// Before
throw std::runtime_error("Failed to navigate to URL");

// After
throw palantir::exception::NavigationException("Failed to navigate to URL");
```

### With Stack Traces

For enhanced debugging, use the traceable version:

```cpp
throw palantir::exception::TraceableNavigationException("Failed to navigate to URL");
```

## Exception Handling

The application's main function includes a catch block that handles these exceptions, including displaying the stack trace in debug builds:

```cpp
try {
    // Application code
} catch (const palantir::exception::TraceableBaseException& e) {
    DebugLog("Fatal error: ", e.what());
    DebugLog(e.getStackTraceString());
    // Platform-specific error reporting
    return 1;
} catch (const std::exception& e) {
    DebugLog("Fatal error: {}", e.what());
    // Fallback error handling
    return 1;
}
```

## Compiler Support Detection

The system automatically detects compiler capabilities at CMake configuration time using the `setup_modern_exception.cmake` module:

```cmake
function(setup_modern_exception)
    # Check compiler ID and version
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "13")
            message(STATUS "Enabling stacktrace support for GCC")
            set(STACKTRACE_SUPPORT_ENABLED 1)
            add_compile_definitions(_GLIBCXX_HAVE_STACKTRACE=1)
            set(CMAKE_CXX_STANDARD 23)
        endif()
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "19.35") # VS 2022 version 17.5
            message(STATUS "Enabling stacktrace support for MSVC")
            set(STACKTRACE_SUPPORT_ENABLED 1)
            add_compile_definitions(_GLIBCXX_HAVE_STACKTRACE=1)
            set(CMAKE_CXX_STANDARD 23)
            # Enable latest C++ features for MSVC
            add_compile_options(/std:c++latest)
        endif()
    endif()
    # ...
endfunction()
```

## Benefits

1. **Improved Error Handling**: More specific exception types make error handling more precise
2. **Enhanced Debugging**: Stack traces provide valuable context for debugging
3. **Cross-Platform Compatibility**: Works across all supported platforms
4. **Future-Proof Design**: Leverages modern C++ features when available
5. **Consistent Error Messages**: Standardized error messaging throughout the application 