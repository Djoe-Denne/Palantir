# Input System Technical Documentation

## Architecture Overview

The input system follows a modular, layered architecture that separates concerns between input detection, signal processing, and command execution. The system employs the Factory and Singleton patterns for component creation, management, and configuration.

### Design Principles

1. **Separation of Concerns**
   - Input detection is isolated from command execution
   - Platform-specific code is encapsulated in dedicated implementations
   - Configuration is separated from business logic
   - Implementation details hidden through PIMPL pattern

2. **Extensibility**
   - New commands can be added without modifying existing code
   - Platform-specific implementations can be added through abstraction layers
   - Input detection mechanisms can be changed independently

3. **Configuration-Driven**
   - All shortcuts are defined in external configuration
   - Runtime behavior is determined by configuration
   - Default configurations are provided automatically

4. **Thread Safety**
   - Singleton instances are thread-safe
   - Key registration is synchronized
   - Signal processing is protected

### Core Components

#### 1. Input Layer
- **IInput Interface**: Defines the contract for input detection
  - Abstracts platform-specific input mechanisms
  - Provides key and modifier state detection
  - Allows for different input implementations

- **ConfigurableInput**: Implementation for keyboard shortcuts
  - Handles key and modifier combinations
  - Platform-agnostic interface
  - PIMPL pattern for platform-specific details

- **KeyRegister**: Singleton key registration service
  - Thread-safe key code registration
  - Platform-specific key code mapping
  - PIMPL pattern for implementation details
  - Instance-based access through getInstance()

- **KeyMapper**: Mapping service for key configurations
  - Converts string representations to platform codes
  - Validates key and modifier combinations
  - Uses KeyRegister singleton for code lookup

- **KeyConfig**: Configuration management
  - Parses INI format configuration files
  - Validates configuration syntax
  - Provides default configurations

#### 2. Signal Layer
- **ISignal Interface**: Defines signal processing contract
  - Lifecycle management (start/stop)
  - State tracking
  - Input checking

- **Signal**: Generic signal implementation
  - Couples inputs with commands
  - Handles debouncing
  - Manages signal state

- **SignalManager**: Signal orchestration
  - Manages signal lifecycle
  - Processes active signals
  - Handles signal registration

- **SignalFactory**: Signal creation service
  - Creates signals from configuration
  - Pairs inputs with commands
  - Validates signal configurations

#### 3. Command Layer
- **ICommand Interface**: Command execution contract
  - Defines execution behavior
  - Provides command identification
  - Ensures consistent command interface

- **Command Types**
  - Generic command implementation
  - Specialized commands (Stop, Show)
  - Extensible command system

### Platform-Specific Implementation

#### Windows Architecture
- Low-level keyboard hook system
- Global event monitoring
- System-wide input capture
- Event-driven signal processing

#### macOS Architecture
- Event monitor system
- Global and local event handling
- Accessibility integration
- Asynchronous event processing

### Factory Pattern Implementation

The system uses two main factories:

1. **InputFactory**
   - Singleton pattern for configuration
   - Static factory methods
   - Configuration-based input creation

2. **SignalFactory**
   - Static factory implementation
   - Command-input pairing
   - Signal lifecycle management

### Configuration Flow

1. **Initialization Phase**
   - Configuration file loading
   - InputFactory initialization
   - Manager creation and setup

2. **Signal Creation Phase**
   - Configuration parsing
   - Input creation
   - Command pairing
   - Signal registration

3. **Runtime Phase**
   - Event capture
   - Signal processing
   - Command execution

### Error Handling Strategy

1. **Configuration Errors**
   - Validation at load time
   - Descriptive error messages
   - Default configuration fallback

2. **Runtime Errors**
   - Platform-specific error handling
   - Graceful degradation
   - Error logging and reporting

3. **Input Validation**
   - Key combination validation
   - Platform compatibility checking
   - Configuration syntax verification

### Key Mapping System

The `KeyRegister` singleton provides platform-independent key mapping:
```cpp
// Get the KeyRegister instance
auto& keyRegister = KeyRegister::getInstance();

// Register platform-specific keys
keyRegister.registerKey("F1", KeyCodes::KEY_F1);
keyRegister.registerKey("/", KeyCodes::KEY_SLASH);

// Register platform-specific modifiers
#ifdef _WIN32
keyRegister.registerKey("CTRL", KeyCodes::CONTROL_MODIFIER);
keyRegister.registerKey("WIN", VK_LWIN);
#else
keyRegister.registerKey("CTRL", KeyCodes::CONTROL_MODIFIER);
keyRegister.registerKey("CMD", KeyCodes::COMMAND_MODIFIER);
#endif
```

The `KeyMapper` uses the `KeyRegister` singleton for lookups:
```cpp
auto& keyRegister = KeyRegister::getInstance();
if (keyRegister.hasKey(upperKey)) {
    return keyRegister.get(upperKey);
}
``` 