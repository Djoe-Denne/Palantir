# Interview Cheater Documentation

## Overview

This documentation provides comprehensive information about the Interview Cheater application, its architecture, build system, and usage. The documentation is organized into several sections covering both development and user aspects.

## Documentation Structure

### Build System and Tools
- [CMake Configuration](technical/cmake-configuration.md): Project structure, build system setup, and configuration
- [Clang Tools](technical/clang-tools.md): Code formatting, static analysis, and development tools
- [GitHub Actions](technical/github-actions.md): CI/CD pipeline configuration and workflows

### Technical Documentation
Located in `/docs/technical/`, this section contains detailed information about the application's architecture:

- [Input System](technical/input_system.md): Detailed architecture of the input handling system, including:
  - Component architecture and design principles
  - Input detection and processing
  - Signal system and command execution
  - Platform-specific implementations
  - Configuration management

### User Documentation
Located in `/docs/user/`, this section provides guides for end users:

- [Shortcuts Configuration](user/shortcuts.md): How to configure and customize keyboard shortcuts

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