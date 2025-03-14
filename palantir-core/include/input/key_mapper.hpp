/**
 * @file key_mapper.hpp
 * @brief Defines the key mapping utility for keyboard input handling.
 *
 * This file contains the KeyMapper class which provides utilities for mapping
 * between string representations of keys/modifiers and their corresponding
 * platform-specific key codes.
 */

#ifndef KEY_MAPPER_HPP
#define KEY_MAPPER_HPP

#include <cstdint>
#include <string>

#include "core_export.hpp"

namespace palantir::input {

/**
 * @class KeyMapper
 * @brief Utility class for mapping between key names and key codes.
 *
 * This static utility class provides methods for converting between human-readable
 * key names and platform-specific key codes. It handles both regular keys and
 * modifier keys, providing validation and mapping services. The class is
 * non-instantiable and provides only static methods.
 */
class PALANTIR_CORE_API KeyMapper {
public:
    /** @brief Deleted constructor to prevent instantiation. */
    KeyMapper() = delete;
    /** @brief Deleted destructor to prevent instantiation. */
    ~KeyMapper() = delete;

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent instantiation. */
    KeyMapper(const KeyMapper&) = delete;
    /** @brief Deleted copy assignment to prevent instantiation. */
    auto operator=(const KeyMapper&) -> KeyMapper& = delete;

    // Delete move operations
    /** @brief Deleted move constructor to prevent instantiation. */
    KeyMapper(KeyMapper&&) = delete;
    /** @brief Deleted move assignment to prevent instantiation. */
    auto operator=(KeyMapper&&) -> KeyMapper& = delete;

    /**
     * @brief Convert a key name to its corresponding key code.
     * @param keyName String representation of the key.
     * @return Platform-specific key code for the given key name.
     *
     * Converts a human-readable key name (e.g., "F1", "/") to the corresponding
     * platform-specific key code.
     * @throws std::runtime_error if the key name is invalid.
     */
    [[nodiscard]] static auto getKeyCode(const std::string& keyName) -> int;

    /**
     * @brief Convert a modifier name to its corresponding modifier code.
     * @param modifierName String representation of the modifier.
     * @return Platform-specific modifier code for the given modifier name.
     *
     * Converts a human-readable modifier name (e.g., "Ctrl", "Win", "Cmd") to
     * the corresponding platform-specific modifier code.
     * @throws std::runtime_error if the modifier name is invalid.
     */
    [[nodiscard]] static auto getModifierCode(const std::string& modifierName) -> int;

    /**
     * @brief Check if a key name is valid.
     * @param keyName String representation of the key to validate.
     * @return true if the key name is valid, false otherwise.
     *
     * Validates whether a given key name is recognized by the system and
     * can be mapped to a valid key code.
     */
    [[nodiscard]] static auto isValidKey(const std::string& keyName) -> bool;

    /**
     * @brief Check if a modifier name is valid.
     * @param modifierName String representation of the modifier to validate.
     * @return true if the modifier name is valid, false otherwise.
     *
     * Validates whether a given modifier name is recognized by the system and
     * can be mapped to a valid modifier code.
     */
    [[nodiscard]] static auto isValidModifier(const std::string& modifierName) -> bool;
};

}  // namespace palantir::input

#endif  // KEY_MAPPER_HPP