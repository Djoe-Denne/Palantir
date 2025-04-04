# Keyboard Shortcuts Configuration Guide

## Overview

The application uses a simple INI file format to configure keyboard shortcuts. The configuration file is located at `config/shortcuts.ini`. If this file doesn't exist, it will be created automatically with default settings.

## Configuration File Format

The shortcuts configuration uses the following format:
```ini
[commands]
command_name = modifier+key    ; Optional comment
```

### Example Configuration
```ini
[commands]
toggle = Ctrl+Num 1    ; Toggle window visibility
stop = Win+/        ; Stop application
```

## Available Commands

The application supports a plugin-based command system, allowing for easy extension with new commands. Currently supported built-in commands:
- `toggle`: Shows/hides the application window
- `stop`: Exits the application
- `window-screenshot`: Takes a screenshot of the current window
- `toggle-transparency`: Toggles window transparency
- `toggle-window-anonymity`: Toggles window anonymity mode
- `send-sauron-implement-request`: Sends a request to Sauron to implement code
- `send-sauron-fix-errors-request`: Sends a request to Sauron to fix errors
- `send-sauron-validate-with-tests-request`: Sends a request to Sauron to validate with tests
- `send-sauron-fix-test-failures-request`: Sends a request to Sauron to fix test failures
- `send-sauron-handle-todos-request`: Sends a request to Sauron to handle TODOs
- `clear-screenshot`: Clears the screenshot folder

### Adding Custom Commands

New commands can be added to the system by:
1. Creating a new command implementation (see [Command System](../technical/command_system.md))
2. Adding a shortcut configuration in `shortcuts.ini`

## Available Modifiers

### Windows
- `Ctrl`: Control key
- `Win`: Windows key (both left and right)
- `Alt`: Alt key
- `Shift`: Shift key

### macOS
- `Ctrl`: Control key
- `Cmd`: Command (⌘) key
- `Alt`: Option/Alt key
- `Shift`: Shift key

## Available Keys

Common keys that can be used:
- Function keys: `F1` through `F12`
- Special characters: `/` (forward slash)
- Letters: `A` through `Z`
- Numbers: `0` through `9`
- Numpad: `Num 0` through `Num 9`

## Configuration Rules

1. Each command must be on a new line
2. The format must be `command = modifier+key`
3. Spaces around `=` and `+` are optional
4. Comments start with `;` and are optional
5. Commands are case-insensitive
6. The `[commands]` section header is required

## Examples

### Basic Configuration
```ini
[commands]
toggle = Ctrl+Num 1
stop = Win+/
```

### With Comments
```ini
; Keyboard shortcuts configuration
[commands]
toggle = Ctrl+Num 1    ; Show/hide window
stop = Win+/        ; Exit application
```

### macOS Configuration
```ini
[commands]
toggle = Ctrl+Num 1    ; Show/hide window
stop = Cmd+/        ; Exit application
```

## Troubleshooting

### Common Issues

1. **Shortcut not working**
   - Check if the modifier and key names are spelled correctly
   - Verify the command name matches exactly
   - Ensure there's no typo in the `+` symbol
   - Verify the command is properly registered in the system

2. **Configuration not loading**
   - Verify the file is in `config/shortcuts.ini`
   - Check file permissions
   - Ensure the file is saved in UTF-8 encoding

3. **Invalid configuration**
   - The application will log errors for invalid configurations
   - Check the application logs for detailed error messages
   - Invalid configurations will prevent the command from being registered

### Default Configuration

If the configuration file is missing or corrupted, the application will create a default configuration:

```ini
; Default keyboard shortcuts configuration
; Format: command = modifier+key
; Available modifiers: Ctrl, Alt, Shift, Win (Windows) / Cmd (macOS)

[commands]
toggle = Ctrl+Num 1    ; Toggle window visibility
stop = Win+/        ; Stop application
window-screenshot = Ctrl+Num 2    ; Take screenshot of current window
toggle-transparency = Ctrl+Num 3    ; Toggle window transparency
send-sauron-implement-request = Ctrl+Num 4    ; Send request to Sauron to implement
send-sauron-fix-errors-request = Ctrl+Num 5    ; Send request to Sauron to fix errors
send-sauron-validate-with-tests-request = Ctrl+Num 6    ; Send request to Sauron to validate with tests
send-sauron-fix-test-failures-request = Ctrl+Num 7    ; Send request to Sauron to fix test failures
send-sauron-handle-todos-request = Ctrl+Num 8    ; Send request to Sauron to handle todos
clear-screenshot = Ctrl+Num 9    ; Clear screenshot folder
```

## Adding New Commands

To add new commands:
1. Create a new command implementation following the [Command System](../technical/command_system.md) guide
2. Open `config/shortcuts.ini`
3. Add a new line under the `[commands]` section
4. Follow the format: `command_name = modifier+key`
5. Save the file
6. Restart the application

The application will validate new commands against its internal command registry and automatically create appropriate input handlers for registered commands.

# Keyboard Shortcuts

This document describes all available keyboard shortcuts in the application.

## Global Shortcuts

| Shortcut | Windows | Description |
|----------|---------|-------------|
| Toggle Window | `Ctrl+Num 1` | Toggle main window visibility |
| Stop Application | `Win+/` | Stop the application |
| Window Screenshot | `Ctrl+Num 2` | Take a screenshot of the current window |
| Toggle Transparency | `Ctrl+Num 3` | Toggle window transparency |
| Send Implement Request | `Ctrl+Num 4` | Send request to Sauron to implement code |
| Send Fix Errors Request | `Ctrl+Num 5` | Send request to Sauron to fix errors |
| Send Validate with Tests Request | `Ctrl+Num 6` | Send request to Sauron to validate with tests |
| Send Fix Test Failures Request | `Ctrl+Num 7` | Send request to Sauron to fix test failures |
| Send Handle TODOs Request | `Ctrl+Num 8` | Send request to Sauron to handle TODOs |
| Clear Screenshot Folder | `Ctrl+Num 9` | Clear the screenshot folder |