#ifndef MACOS_KEY_CODES_HPP
#define MACOS_KEY_CODES_HPP

namespace KeyCodes {
    // Virtual key codes for macOS
    // Reference: https://stackoverflow.com/questions/3202629/where-can-i-find-a-list-of-mac-virtual-key-codes
    constexpr int KEY_SLASH = 44;  // Forward slash (/) key - virtual keycode 44
    constexpr int COMMAND_MODIFIER = 0x100000;  // Command (⌘) key modifier
}

#endif // MACOS_KEY_CODES_HPP 