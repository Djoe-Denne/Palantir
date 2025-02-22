# Command System

## Overview

The command system provides a flexible and extensible way to add new commands to the application. It uses a factory pattern combined with automatic registration to make command creation straightforward and maintainable.

## Architecture

### Core Components

1. `ICommand` Interface
   - Base interface for all commands
   - Defines the `execute()` method that all commands must implement
   - `useDebounce()` method to determine if the command should use debounce
   - Supports move semantics but prohibits copying
2. `CommandFactory`
   - Static factory class for managing and creating commands
   - Maintains a registry of command creators
   - Provides methods for registering and retrieving commands

3. `AutoCommandRegister` Utility
   - Macro-based automatic command registration
   - Simplifies adding new commands to the system

## Creating New Commands

### Step 1: Define Command Class

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

### Step 2: Implement Command

Create the implementation file:

```cpp
#include "command/your_command.hpp"
#include "utils/auto_command_register.hpp"

namespace interview_cheater::command {

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

}  // namespace interview_cheater::command

// Register command with a unique name
REGISTER_COMMAND("your_command_name", your::namespace::, YourCommand)
```

### Step 3: Add to Build System

Add your command to `common-source.cmake`:

```cmake
set(COMMAND_SOURCES
    ${COMMAND_SOURCES}
    ${PROJECT_ROOT}/src/command/your_command.cpp
)
```

## Command Registration

The `REGISTER_COMMAND` macro automatically:
1. Creates a command factory function
2. Registers the command with the `CommandFactory`
3. Ensures registration happens at program startup

Example:
```cpp
REGISTER_COMMAND("stop", interview_cheater::command, StopCommand)
```

This expands to create:
1. A factory function to create the command
2. An anonymous namespace to prevent symbol conflicts
3. A static registration that happens before main()

## Using Commands

Commands can be retrieved and executed through:

1. Direct Factory Access:
```cpp
auto command = CommandFactory::getCommand("command_name");
if (command) {
    command->execute();
}
```

2. Signal System:
```cpp
auto input = InputFactory::createInput("command_name");
auto command = CommandFactory::getCommand("command_name");
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
7. Use dependency injection for required services 