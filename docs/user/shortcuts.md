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
toggle = Ctrl+F1    ; Toggle window visibility
stop = Win+/        ; Stop application
```

## Available Commands

Currently supported commands:
- `toggle`: Shows/hides the application window
- `stop`: Exits the application

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
toggle = Ctrl+F1
stop = Win+/
```

### With Comments
```ini
; Keyboard shortcuts configuration
[commands]
toggle = Ctrl+F1    ; Show/hide window
stop = Win+/        ; Exit application
```

### macOS Configuration
```ini
[commands]
toggle = Ctrl+F1    ; Show/hide window
stop = Cmd+/        ; Exit application
```

## Troubleshooting

### Common Issues

1. **Shortcut not working**
   - Check if the modifier and key names are spelled correctly
   - Verify the command name matches exactly
   - Ensure there's no typo in the `+` symbol

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
toggle = Ctrl+F1    ; Toggle window visibility
stop = Win+/        ; Stop application
```

## Adding New Commands

To add new commands:
1. Open `config/shortcuts.ini`
2. Add a new line under the `[commands]` section
3. Follow the format: `command_name = modifier+key`
4. Save the file
5. Restart the application

The application will validate new commands against its internal command registry. 