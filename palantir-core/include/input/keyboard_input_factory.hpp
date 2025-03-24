/**
 * @file keyboard_input_factory.hpp
 * @brief Defines the factory for creating keyboard input handlers.
 *
 * This file contains the KeyboardInputFactory class which is responsible for creating
 * keyboard input handlers based on configuration.
 */

#ifndef KEYBOARD_INPUT_FACTORY_HPP
#define KEYBOARD_INPUT_FACTORY_HPP

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "core_export.hpp"
#include "concept/input_concept.hpp"
#include "config/config.hpp"
#include "input/keyboard_Input.hpp"
#include "input/key_config.hpp"
#include "input/key_mapper.hpp"

namespace palantir::input {

/**
 * @class KeyboardInputFactory
 * @brief Factory class for creating keyboard input handlers.
 *
 * This factory class is responsible for creating and managing keyboard input
 * handlers based on configuration. It provides a centralized way to create
 * input handlers and manage their configuration.
 */
class PALANTIR_CORE_API KeyboardInputFactory {
public:
    KeyboardInputFactory(const config::Config& config);
    virtual ~KeyboardInputFactory() = default;

    // Delete copy operations
    KeyboardInputFactory(const KeyboardInputFactory&) = delete;
    auto operator=(const KeyboardInputFactory&) -> KeyboardInputFactory& = delete;

    // Delete move operations
    KeyboardInputFactory(KeyboardInputFactory&&) = delete;
    auto operator=(KeyboardInputFactory&&) -> KeyboardInputFactory& = delete;

    /**
     * @brief Create a keyboard input handler for a specific command.
     * @param commandName Name of the command to create input for.
     * @return A unique pointer to the created input handler.
     *
     * Creates and returns an input handler configured for the specified command.
     * The configuration is loaded from the initialized configuration file.
     * @throws std::runtime_error if the factory is not initialized or the command
     * is not found in configuration.
     */
    [[nodiscard]] virtual auto createInput(const std::string& commandName) const -> std::unique_ptr<KeyboardInput>;

    /**
     * @brief Check if a shortcut exists for a command.
     * @param commandName Name of the command to check.
     * @return true if a shortcut exists, false otherwise.
     *
     * Checks if there is a configured shortcut for the specified command.
     * @throws std::runtime_error if the factory is not initialized.
     */
    [[nodiscard]] virtual auto hasShortcut(const std::string& commandName) const -> bool;

    /**
     * @brief Get a list of all configured commands.
     * @return Vector of command names that have shortcuts configured.
     *
     * Returns a list of all commands that have shortcuts configured in the
     * configuration file.
     * @throws std::runtime_error if the factory is not initialized.
     */
    [[nodiscard]] virtual auto getConfiguredCommands() const -> std::vector<std::string>;

private:
    std::unique_ptr<KeyConfig> keyConfig_;

    /**
     * @brief Create a default configuration file.
     * @param configPath Path where the default configuration should be created.
     *
     * Creates a default configuration file with platform-specific shortcuts
     * when no configuration file exists.
     * @throws palantir::exception::TraceableConfigFileException if the file cannot be created or written to.
     */
    auto createDefaultConfig(const std::filesystem::path& configPath) const -> void;
};

}  // namespace palantir::input

#endif  // KEYBOARD_INPUT_FACTORY_HPP 