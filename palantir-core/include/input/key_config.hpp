/**
 * @file key_config.hpp
 * @brief Defines the configuration system for keyboard shortcuts.
 *
 * This file contains the KeyConfig class and ShortcutConfig struct which together
 * manage the loading, storage, and access of keyboard shortcut configurations
 * from INI files.
 */

#ifndef KEY_CONFIG_HPP
#define KEY_CONFIG_HPP

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "core_export.hpp"
#include "utils/string_utils.hpp"

namespace palantir::input {

/**
 * @struct ShortcutConfig
 * @brief Structure representing a keyboard shortcut configuration.
 *
 * This structure holds the components of a keyboard shortcut, including
 * the modifier key (e.g., Ctrl, Alt) and the main key. It provides a
 * simple data container for shortcut configurations.
 */
struct PALANTIR_CORE_API ShortcutConfig {
    /** @brief The modifier key name (e.g., "Ctrl", "Alt", "Win", "Cmd"). */
    std::string modifier;
    /** @brief The main key name (e.g., "F1", "/", "A"). */
    std::string key;
};

/**
 * @class KeyConfig
 * @brief Class for managing keyboard shortcut configurations.
 *
 * This class is responsible for loading and managing keyboard shortcut
 * configurations from INI files. It provides methods to access and query
 * the loaded configurations, supporting the application's input system.
 */
class PALANTIR_CORE_API KeyConfig {
public:
    /**
     * @brief Construct a new KeyConfig object.
     * @param configPath Path to the INI configuration file.
     *
     * Loads and parses the shortcut configuration from the specified INI file.
     * The file should contain shortcut definitions in the format:
     * [commands]
     * command_name = modifier+key
     * @throws std::runtime_error if the configuration file cannot be loaded or parsed.
     */
    explicit KeyConfig(const std::filesystem::path& configPath);

    /**
     * @brief Destructor for KeyConfig.
     */
    virtual ~KeyConfig() = default;

    // Delete copy operations
    KeyConfig(const KeyConfig&) = delete;
    auto operator=(const KeyConfig&) -> KeyConfig& = delete;

    // Delete move operations
    KeyConfig(KeyConfig&&) noexcept = delete;
    auto operator=(KeyConfig&&) noexcept -> KeyConfig& = delete;

    /**
     * @brief Get the shortcut configuration for a command.
     * @param commandName Name of the command to get the shortcut for.
     * @return const reference to the ShortcutConfig for the command.
     *
     * Retrieves the shortcut configuration for a specific command.
     * @throws std::runtime_error if the command is not found in configuration.
     */
    [[nodiscard]] virtual auto getShortcut(const std::string& commandName) const -> const ShortcutConfig&;

    /**
     * @brief Check if a shortcut exists for a command.
     * @param commandName Name of the command to check.
     * @return true if a shortcut exists for the command, false otherwise.
     *
     * Checks whether a shortcut configuration exists for the specified command.
     */
    [[nodiscard]] virtual auto hasShortcut(const std::string& commandName) const -> bool;

    /**
     * @brief Get a list of all configured commands.
     * @return Vector of command names that have shortcuts configured.
     *
     * Returns a list of all commands that have shortcut configurations.
     * The command names include their section prefix (e.g., "commands.stop").
     */
    [[nodiscard]] virtual auto getConfiguredCommands() const -> std::vector<std::string>;

private:
#pragma warning(push)
#pragma warning(disable : 4251)
    /** @brief Map of command names to their shortcut configurations. */
    std::unordered_map<std::string, ShortcutConfig, utils::StringUtils::StringHash, std::equal_to<>> shortcuts_;
#pragma warning(pop)

    /**
     * @brief Loads and parses the shortcut configuration from the specified INI file.
     * @param configPath Path to the INI configuration file.
     * @throws std::runtime_error if the configuration file cannot be loaded or parsed.
     */
    auto loadConfig(const std::filesystem::path& configPath) -> void;
};

}  // namespace palantir::input

#endif  // KEY_CONFIG_HPP