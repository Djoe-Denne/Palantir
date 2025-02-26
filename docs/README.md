# Interview Cheater Documentation

This directory contains comprehensive documentation for the Interview Cheater application.

## Features

- **Window Management**: Toggle application window visibility
- **Screenshot Capture**: Take screenshots of active windows with automatic file naming
- **Cross-Platform**: Supports both Windows and macOS
- **Plugin System**: Extensible command system for adding new functionality

## Documentation Structure

### User Documentation
- [Keyboard Shortcuts](user/shortcuts.md): List of all keyboard shortcuts and commands

### Technical Documentation
- [Command System](technical/command_system.md): Details about the command system and available commands
- [Plugin System](technical/plugin_system.md): Information about the plugin architecture
- [Input System](technical/input_system.md): Details about keyboard input handling
- [CMake Configuration](technical/cmake-configuration.md): Build system configuration
- [GitHub Actions](technical/github-actions.md): CI/CD pipeline configuration
- [Clang Tools](technical/clang-tools.md): Code formatting and static analysis

## Getting Started

1. Review the [Keyboard Shortcuts](user/shortcuts.md) for basic usage
2. Check the [Command System](technical/command_system.md) for available commands
3. See [Plugin System](technical/plugin_system.md) for extending functionality

## Development

For development-related information, refer to:
- [CMake Configuration](technical/cmake-configuration.md)
- [Clang Tools](technical/clang-tools.md)
- [GitHub Actions](technical/github-actions.md)

## Architecture

The application follows a plugin-based architecture with these key components:
- Core command system
- Platform-specific implementations
- Plugin management
- Input handling and key mapping

See the [architecture](technical/architecture/) directory for detailed design documents.

## Quick Start

### For Users
1. Install the application
2. Configure shortcuts using the [Shortcuts Configuration Guide](user/shortcuts.md)

### For Developers
1. Review the [CMake Configuration](cmake-configuration.md) for build setup
2. Set up development tools using the [Clang Tools Guide](clang-tools.md)
3. Review technical documentation to understand the architecture

## Contributing

When contributing to the project:

1. Follow the [CMake Configuration Guide](cmake-configuration.md) for adding new source files
2. Ensure your code passes both format and lint checks as described in [Clang Tools](clang-tools.md)
3. Follow the existing architectural patterns documented in technical documentation
4. Add appropriate documentation for new features

## Support

If you encounter issues:
1. Check the relevant documentation sections
2. Review build and tool configuration
3. Submit an issue with detailed information about your problem 