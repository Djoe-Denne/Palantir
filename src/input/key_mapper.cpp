#include "input/key_mapper.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "input/key_codes.hpp"
#include "utils/logger.hpp"

namespace interview_cheater::input {
// Helper function to convert string to uppercase
auto toUpper(std::string str) -> std::string {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Key name to key code mapping
const std::unordered_map<std::string, int> keyMap = {
    // Letters
    {"A", KeyCodes::KEY_A},
    {"B", KeyCodes::KEY_B},
    {"C", KeyCodes::KEY_C},
    {"D", KeyCodes::KEY_D},
    {"E", KeyCodes::KEY_E},
    {"F", KeyCodes::KEY_F},
    {"G", KeyCodes::KEY_G},
    {"H", KeyCodes::KEY_H},
    {"I", KeyCodes::KEY_I},
    {"J", KeyCodes::KEY_J},
    {"K", KeyCodes::KEY_K},
    {"L", KeyCodes::KEY_L},
    {"M", KeyCodes::KEY_M},
    {"N", KeyCodes::KEY_N},
    {"O", KeyCodes::KEY_O},
    {"P", KeyCodes::KEY_P},
    {"Q", KeyCodes::KEY_Q},
    {"R", KeyCodes::KEY_R},
    {"S", KeyCodes::KEY_S},
    {"T", KeyCodes::KEY_T},
    {"U", KeyCodes::KEY_U},
    {"V", KeyCodes::KEY_V},
    {"W", KeyCodes::KEY_W},
    {"X", KeyCodes::KEY_X},
    {"Y", KeyCodes::KEY_Y},
    {"Z", KeyCodes::KEY_Z},

    // Numbers
    {"0", KeyCodes::KEY_0},
    {"1", KeyCodes::KEY_1},
    {"2", KeyCodes::KEY_2},
    {"3", KeyCodes::KEY_3},
    {"4", KeyCodes::KEY_4},
    {"5", KeyCodes::KEY_5},
    {"6", KeyCodes::KEY_6},
    {"7", KeyCodes::KEY_7},
    {"8", KeyCodes::KEY_8},
    {"9", KeyCodes::KEY_9},

    // Non-alphanumeric characters
    {"/", KeyCodes::KEY_SLASH},

    // Function keys
    {"F1", KeyCodes::KEY_F1},
    {"F2", KeyCodes::KEY_F2},
    {"F3", KeyCodes::KEY_F3},
    {"F4", KeyCodes::KEY_F4},
    {"F5", KeyCodes::KEY_F5},
    {"F6", KeyCodes::KEY_F6},
    {"F7", KeyCodes::KEY_F7},
    {"F8", KeyCodes::KEY_F8},
    {"F9", KeyCodes::KEY_F9},
    {"F10", KeyCodes::KEY_F10},
    {"F11", KeyCodes::KEY_F11},
    {"F12", KeyCodes::KEY_F12},
    {"F13", KeyCodes::KEY_F13},
    {"F14", KeyCodes::KEY_F14},
    {"F15", KeyCodes::KEY_F15},
    {"F16", KeyCodes::KEY_F16},
    {"F17", KeyCodes::KEY_F17},
    {"F18", KeyCodes::KEY_F18},
    {"F19", KeyCodes::KEY_F19},
    {"F20", KeyCodes::KEY_F20},
    {"F21", KeyCodes::KEY_F21},
    {"F22", KeyCodes::KEY_F22},
    {"F23", KeyCodes::KEY_F23},
    {"F24", KeyCodes::KEY_F24},

    // Special keys
    {"ESCAPE", KeyCodes::KEY_ESCAPE},
    {"TAB", KeyCodes::KEY_TAB},
    {"CAPSLOCK", KeyCodes::KEY_CAPSLOCK},
    {"SPACE", KeyCodes::KEY_SPACE},
    {"BACKSPACE", KeyCodes::KEY_BACKSPACE},
    {"ENTER", KeyCodes::KEY_ENTER},
    {"DELETE", KeyCodes::KEY_DELETE},
    {"INSERT", KeyCodes::KEY_INSERT},
    {"HOME", KeyCodes::KEY_HOME},
    {"END", KeyCodes::KEY_END},
    {"PAGEUP", KeyCodes::KEY_PAGEUP},
    {"PAGEDOWN", KeyCodes::KEY_PAGEDOWN},
    {"PRINTSCREEN", KeyCodes::KEY_PRINTSCREEN},
    {"SCROLLLOCK", KeyCodes::KEY_SCROLLLOCK},
    {"PAUSE", KeyCodes::KEY_PAUSE},
    {"NUMLOCK", KeyCodes::KEY_NUMLOCK},

    // Arrow keys
    {"UP", KeyCodes::KEY_UP},
    {"DOWN", KeyCodes::KEY_DOWN},
    {"LEFT", KeyCodes::KEY_LEFT},
    {"RIGHT", KeyCodes::KEY_RIGHT},
};

// Modifier name to modifier code mapping
const std::unordered_map<std::string, int> modifierMap = {
#ifdef _WIN32
    {"CTRL", KeyCodes::CONTROL_MODIFIER},
    {"WIN", KeyCodes::WINDOWS_MODIFIER},  // Left Windows key
    {"ALT", KeyCodes::ALT_MODIFIER},      // Alt key
    {"SHIFT", KeyCodes::SHIFT_MODIFIER}
#else
    {"CTRL", KeyCodes::CONTROL_MODIFIER},
    {"CMD", KeyCodes::COMMAND_MODIFIER},
    {"ALT", KeyCodes::ALT_MODIFIER},
    {"SHIFT", KeyCodes::SHIFT_MODIFIER}
#endif
};

/**
 * @brief Get the virtual key code for a given key name.
 * @param keyName The name of the key to look up.
 * @return The virtual key code corresponding to the key name.
 * @throws std::invalid_argument if the key name is not recognized.
 *
 * Looks up the virtual key code for a given key name in the key map.
 * The key name must be one of the predefined values in the key map.
 */
auto KeyMapper::getKeyCode(const std::string& keyName) -> int {
    DEBUG_LOG("Looking up key code for: {}", keyName);

    const auto upperKey = toUpper(keyName);
    auto it = keyMap.find(upperKey);
    if (it == keyMap.end()) {
        DEBUG_LOG("Invalid key name: {}", upperKey);
        throw std::invalid_argument("Invalid key name: " + keyName);
    }

    DEBUG_LOG("Found key code: 0x{:x}", it->second);
    return it->second;
}

/**
 * @brief Get the virtual key code for a given modifier name.
 * @param modifierName The name of the modifier to look up.
 * @return The virtual key code corresponding to the modifier name.
 * @throws std::invalid_argument if the modifier name is not recognized.
 *
 * Looks up the virtual key code for a given modifier name in the modifier map.
 * The modifier name must be one of the predefined values in the modifier map.
 */
auto KeyMapper::getModifierCode(const std::string& modifierName) -> int {
    DEBUG_LOG("Looking up modifier code for: {}", modifierName);

    const auto upperModifier = toUpper(modifierName);
    auto it = modifierMap.find(upperModifier);
    if (it == modifierMap.end()) {
        DEBUG_LOG("Invalid modifier name: {}", upperModifier);
        throw std::invalid_argument("Invalid modifier name: " + modifierName);
    }

    DEBUG_LOG("Found modifier code: 0x{:x}", it->second);
    return it->second;
}

/**
 * @brief Check if a given key name is valid.
 * @param keyName The name of the key to validate.
 * @return true if the key name is valid, false otherwise.
 *
 * Checks if a given key name exists in the key map. This can be used
 * to validate key names before attempting to get their key codes.
 */
auto KeyMapper::isValidKey(const std::string& keyName) -> bool {
    const auto upperKey = toUpper(keyName);
    bool valid = keyMap.find(upperKey) != keyMap.end();
    DEBUG_LOG("Key name '{}' is {}", upperKey, valid ? "valid" : "invalid");
    return valid;
}

/**
 * @brief Check if a given modifier name is valid.
 * @param modifierName The name of the modifier to validate.
 * @return true if the modifier name is valid, false otherwise.
 *
 * Checks if a given modifier name exists in the modifier map. This can be used
 * to validate modifier names before attempting to get their modifier codes.
 */
auto KeyMapper::isValidModifier(const std::string& modifierName) -> bool {
    const auto upperModifier = toUpper(modifierName);
    bool valid = modifierMap.find(upperModifier) != modifierMap.end();
    DEBUG_LOG("Modifier name '{}' is {}", upperModifier, valid ? "valid" : "invalid");
    return valid;
}

}  // namespace interview_cheater::input