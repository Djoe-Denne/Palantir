# Plugin System

## Overview

The plugin system provides a dynamic way to extend the application's functionality at runtime through loadable modules. It supports hot-loading of plugins and manages their lifecycle.

## Architecture

### Core Components

1. `IPlugin` Interface
   - Base interface for all plugins
   - Defines `initialize()` and `shutdown()` methods for lifecycle management
   - Provides `getName()` and `getVersion()` for plugin identification

2. `PluginLoader`
   - Handles loading and unloading of plugin shared libraries
   - Platform-independent implementation (Windows/Unix)
   - Manages plugin creation and destruction functions
   - Provides safe cleanup of loaded libraries

3. `PluginManager`
   - High-level plugin management
   - Handles plugin lifecycle
   - Maintains registry of loaded plugins
   - Provides directory-based plugin loading

## Creating New Plugins

### Step 1: Define Plugin Class

Create a new plugin class that inherits from `IPlugin`:

```cpp
class YourPlugin final : public plugin::IPlugin {
public:
    bool initialize() override;
    void shutdown() override;
    std::string getName() const override;
    std::string getVersion() const override;
private:
    // Add any required member variables
};
```

### Step 2: Implement Plugin

Create the implementation file:

```cpp
#include "your_plugin.hpp"

namespace your::namespace {

bool YourPlugin::initialize() {
    // Initialize your plugin
    return true;
}

void YourPlugin::shutdown() {
    // Cleanup your plugin
}

std::string YourPlugin::getName() const {
    return "Your Plugin Name";
}

std::string YourPlugin::getVersion() const {
    return "1.0.0";
}

}  // namespace your::namespace

// Register the plugin
IMPLEMENT_PLUGIN(your::namespace::YourPlugin)
```

### Step 3: Add to Build System

Add your plugin to CMake:

```cmake
add_library(your-plugin SHARED
    src/your_plugin.cpp
    include/your_plugin.hpp
)

# Required: Link against palantir-core to access IPlugin interface and Palantir API
target_link_libraries(your-plugin PRIVATE palantir-core)
```

Note: All plugins must link against `palantir-core` to implement the `IPlugin` interface and access the Palantir API functionality.

## Plugin Loading

Plugins are loaded at runtime from a designated plugins directory:

```cpp
auto pluginManager = std::make_unique<PluginManager>();
std::filesystem::path pluginsDir = exePath / "plugins";
pluginManager->setupFromDirectory(pluginsDir);
```

## Plugin Lifecycle

1. Loading
   - Plugin shared library is loaded
   - Plugin instance is created
   - Plugin is registered with manager

2. Initialization
   - `initialize()` is called
   - Plugin sets up resources
   - Plugin registers functionality

3. Runtime
   - Plugin is active and functional
   - Can be queried through manager

4. Shutdown
   - `shutdown()` is called
   - Plugin cleans up resources
   - Plugin is unloaded

## Best Practices

1. Keep plugins focused and single-purpose
2. Handle initialization failures gracefully
3. Clean up all resources in shutdown
4. Use meaningful plugin names
5. Follow semantic versioning
6. Document plugin dependencies
7. Use dependency injection when needed 