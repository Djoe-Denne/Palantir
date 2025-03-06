#include "input/key_mapper.hpp"

#include <stdexcept>
#include <string>

#include "input/key_register.hpp"
#include "utils/logger.hpp"
#include "utils/string_utils.hpp"

namespace palantir::input {

/**
 * @brief Get the virtual key code for a given key name.
 * @param keyName The name of the key to look up.
 * @return The virtual key code corresponding to the key name.
 * @throws std::invalid_argument if the key name is not recognized.
 *
 * Looks up the virtual key code for a given key name in the key register.
 * The key name must be one of the predefined values in the key register.
 */
auto KeyMapper::getKeyCode(const std::string& keyName) -> int {
    DEBUG_LOG("Looking up key code for: {}", keyName);

    const auto upperKey = utils::StringUtils::toUpper(keyName);
    if (!KeyRegister::getInstance()->hasKey(upperKey)) {
        DEBUG_LOG("Invalid key name: {}", upperKey);
        throw std::invalid_argument("Invalid key name: " + keyName);
    }

    int keyCode = KeyRegister::getInstance()->get(upperKey);
    DEBUG_LOG("Found key code: 0x{:x}", keyCode);
    return keyCode;
}

/**
 * @brief Get the virtual key code for a given modifier name.
 * @param modifierName The name of the modifier to look up.
 * @return The virtual key code corresponding to the modifier name.
 * @throws std::invalid_argument if the modifier name is not recognized.
 *
 * Looks up the virtual key code for a given modifier name in the key register.
 * The modifier name must be one of the predefined values in the key register.
 */
auto KeyMapper::getModifierCode(const std::string& modifierName) -> int {
    DEBUG_LOG("Looking up modifier code for: {}", modifierName);

    const auto upperModifier = utils::StringUtils::toUpper(modifierName);
    if (!KeyRegister::getInstance()->hasKey(upperModifier)) {
        DEBUG_LOG("Invalid modifier name: {}", upperModifier);
        throw std::invalid_argument("Invalid modifier name: " + modifierName);
    }

    int modifierCode = KeyRegister::getInstance()->get(upperModifier);
    DEBUG_LOG("Found modifier code: 0x{:x}", modifierCode);
    return modifierCode;
}

/**
 * @brief Check if a given key name is valid.
 * @param keyName The name of the key to validate.
 * @return true if the key name is valid, false otherwise.
 *
 * Checks if a given key name exists in the key register. This can be used
 * to validate key names before attempting to get their key codes.
 */
auto KeyMapper::isValidKey(const std::string& keyName) -> bool {
    const auto upperKey = utils::StringUtils::toUpper(keyName);
    bool valid = KeyRegister::getInstance()->hasKey(upperKey);
    DEBUG_LOG("Key name '{}' is {}", upperKey, valid ? "valid" : "invalid");
    return valid;
}

/**
 * @brief Check if a given modifier name is valid.
 * @param modifierName The name of the modifier to validate.
 * @return true if the modifier name is valid, false otherwise.
 *
 * Checks if a given modifier name exists in the key register. This can be used
 * to validate modifier names before attempting to get their modifier codes.
 */
auto KeyMapper::isValidModifier(const std::string& modifierName) -> bool {
    const auto upperModifier = utils::StringUtils::toUpper(modifierName);
    bool valid = KeyRegister::getInstance()->hasKey(upperModifier);
    DEBUG_LOG("Modifier name '{}' is {}", upperModifier, valid ? "valid" : "invalid");
    return valid;
}

}  // namespace palantir::input