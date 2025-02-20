# Project Documentation

## Build System and Tools

This documentation covers the build system configuration and development tools used in the project.

### Contents

1. [CMake Configuration](cmake-configuration.md)
   - Project structure
   - Source organization
   - Platform-specific configuration
   - Adding new source files
   - Build configuration

2. [Clang Tools](clang-tools.md)
   - Setup and configuration
   - Code formatting (clang-format)
   - Static analysis (clang-tidy)
   - Troubleshooting

## Quick Start

### Building the Project

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Development Workflow

1. Write your code
2. Format code: `cmake --build . --target format`
3. Check for issues: `cmake --build . --target lint-check`
4. Fix issues: `cmake --build . --target lint`
5. Build and test

### Available Targets

Run `cmake --build . --target helper` for a complete list of available targets.

## Contributing

When contributing to the project:

1. Follow the [CMake Configuration Guide](cmake-configuration.md) for adding new source files
2. Ensure your code passes both format and lint checks
3. Use the provided targets for code quality:
   ```bash
   cmake --build . --target check-all  # Before committing
   cmake --build . --target fix-all    # To fix issues
   ```

## Need Help?

- Check the [Clang Tools Guide](clang-tools.md) for troubleshooting
- Run the helper target for available commands
- Consult the specific documentation files for detailed information 