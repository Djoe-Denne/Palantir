# Command System

## Overview

The command system provides a flexible and extensible way to add new commands to the application. It uses a factory pattern combined with a plugin-based architecture to make command creation and management straightforward and maintainable.

## Architecture

### Core Components

1. `ICommand` Interface
   - Base interface for all commands
   - Defines the `execute()` method that all commands must implement
   - `useDebounce()` method to determine if the command should use debounce
   - Supports move semantics but prohibits copying

2. `CommandFactory`
   - Singleton factory class for managing and creating commands
   - Uses PIMPL pattern for implementation details
   - Maintains a registry of command creators through its implementation
   - Provides instance methods for registering and retrieving commands
   - Thread-safe singleton access

3. `CommandsPlugin`
   - Plugin that registers all built-in commands
   - Manages command lifecycle through plugin system
   - Handles registration and unregistration of commands

## Creating New Commands

### Step 1: Create a Plugin

First, create a new plugin to host your commands:

```cpp
class YourCommandsPlugin final : public plugin::IPlugin {
public:
    bool initialize() override {
        // Register your commands here
        command::CommandFactory::getInstance().registerCommand("your_command", &createYourCommand);
        return true;
    }

    void shutdown() override {
        // Unregister your commands
        command::CommandFactory::getInstance().unregisterCommand("your_command");
    }

    std::string getName() const override { return "Your Commands Plugin"; }
    std::string getVersion() const override { return "1.0.0"; }
};

IMPLEMENT_PLUGIN(your::namespace::YourCommandsPlugin)
```

### Step 2: Define Command Class

Create a new command class that inherits from `ICommand`:

```cpp
class YourCommand final : public ICommand {
public:
    explicit YourCommand();
    ~YourCommand() override = default;

    // Delete copy operations
    YourCommand(const YourCommand&) = delete;
    auto operator=(const YourCommand&) -> YourCommand& = delete;

    // Define move operations
    YourCommand(YourCommand&&) noexcept = default;
    auto operator=(YourCommand&&) noexcept -> YourCommand& = default;

    auto execute() -> void override;
    auto useDebounce() -> bool override;
private:
    // Add any required member variables
};
```

### Step 3: Implement Command

Create the implementation file:

```cpp
#include "command/your_command.hpp"

namespace your::namespace {

YourCommand::YourCommand() {
    // Initialize your command
}

auto YourCommand::execute() -> void {
    // Implement command logic
}

auto YourCommand::useDebounce() -> bool {
    // Return true if the command should use debounce
    return true;
}

}  // namespace your::namespace
```

### Step 4: Add to Build System

Add your plugin to CMake:

```cmake
add_library(your-commands-plugin SHARED
    src/your_commands_plugin.cpp
    src/your_command.cpp
    include/your_command.hpp
)

target_link_libraries(your-commands-plugin PRIVATE palantir-core)
```

## Command Registration

Commands are now registered through plugins:
1. Plugin is loaded at startup
2. Plugin's `initialize()` registers commands
3. Commands are available through `CommandFactory`
4. Plugin's `shutdown()` unregisters commands

## Using Commands

Commands can be retrieved and executed through:

1. Factory Instance Access:
```cpp
auto& factory = CommandFactory::getInstance();
auto command = factory.getCommand("command_name");
if (command) {
    command->execute();
}
```

2. Signal System:
```cpp
auto input = InputFactory::createInput("command_name");
auto& factory = CommandFactory::getInstance();
auto command = factory.getCommand("command_name");
auto signal = std::make_unique<Signal>(std::move(input), std::move(command));
```

## Adding Keyboard Shortcuts

Commands can be bound to keyboard shortcuts in `config/shortcuts.ini`:

```ini
[commands]
your_command_name = Modifier+Key    ; Description
```

The `InputFactory` will automatically create appropriate input handlers for registered commands.

## Best Practices

1. Make commands focused and single-purpose
2. Use meaningful command names in registration
3. Document command purpose and effects
4. Follow the existing command pattern for consistency
5. Add appropriate error handling in execute()
6. Keep commands stateless when possible
7. Group related commands in a single plugin
8. Handle plugin lifecycle properly

## Built-in Commands

The application comes with several built-in commands:

### Show Command
- **ID**: `toggle`
- **Purpose**: Toggles the visibility of the main application window
- **Implementation**: `ShowCommand` class

### Stop Command
- **ID**: `stop`
- **Purpose**: Stops the application
- **Implementation**: `StopCommand` class

### Window Screenshot Command
- **ID**: `window-screenshot`
- **Purpose**: Captures a screenshot of the currently active window
- **Implementation**: `WindowScreenshotCommand` class
- **Platform-specific**: Uses GDI+ on Windows and CoreGraphics on macOS
- **Output**: Saves PNG files to `./screenshot` directory with timestamp-based names

### Toggle Transparency Command
- **ID**: `toggle-transparency`
- **Purpose**: Toggles the transparency of the main application window between normal (255) and semi-transparent (100)
- **Implementation**: `ToggleTransparencyCommand` class
- **Platform-specific**: Uses GetLayeredWindowAttributes on Windows, platform-specific implementation for macOS
- **Behavior**: Switches between transparency values of 100 and 255

### Toggle Window Anonymity Command
- **ID**: `toggle-window-anonymity`
- **Purpose**: Toggles the window anonymity mode
- **Implementation**: `ToggleWindowAnonymityCommand` class
- **Behavior**: Switches the window between normal and anonymous modes

### Clear Screenshot Command
- **ID**: `clear-screenshot`
- **Purpose**: Clears all screenshots from the screenshot directory
- **Implementation**: `ClearScreenshotCommand` class
- **Behavior**: Deletes all files in the `./screenshot` directory

### Send Sauron Request Command
- **ID**: `send-sauron-implement-request`, `send-sauron-fix-errors-request`, `send-sauron-validate-with-tests-request`, `send-sauron-fix-test-failures-request`, `send-sauron-handle-todos-request`
- **Purpose**: Sends a request to the Sauron AI service with screenshots of the current window
- **Implementation**: `SendSauronRequestCommand` class
- **Behavior**: 
  - Collects all screenshots from the `./screenshot` directory
  - Sends them to the Sauron AI service with a specific prompt
  - Displays the AI response in the application window
- **Variants**:
  - `send-sauron-implement-request`: Asks Sauron to implement code based on comments
  - `send-sauron-fix-errors-request`: Asks Sauron to fix errors shown in the console
  - `send-sauron-validate-with-tests-request`: Asks Sauron to validate code with tests
  - `send-sauron-fix-test-failures-request`: Asks Sauron to fix test failures
  - `send-sauron-handle-todos-request`: Asks Sauron to implement TODOs in the code

## Command Registration

Commands are registered through the `CommandFactory` singleton:

```cpp
command::CommandFactory::getInstance().registerCommand("command-id", &createCommandFunction);
``` 