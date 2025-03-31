/**
 * @file input_factory.hpp
 * @brief Defines the factory for creating input handlers.
 *
 * This file contains the InputFactory class which is responsible for creating
 * input handlers based on configuration. It manages the initialization of the
 * input system and creation of input handlers for different commands.
 */

#ifndef INPUT_FACTORY_HPP
#define INPUT_FACTORY_HPP

#include <memory>
#include <string>

#include "config/config.hpp"
#include "core_export.hpp"
#include "input/iinput.hpp"
#include "input/iinput_factory.hpp"

namespace palantir::input {

/**
 * @class InputFactory
 * @brief Factory class for creating input handlers.
 *
 * This static factory class is responsible for creating and managing input
 * handlers based on configuration. It provides a centralized way to create
 * input handlers and manage their configuration. The class is non-instantiable
 * and provides only static methods.
 */
class PALANTIR_CORE_API KeyboardInputFactory : public IInputFactory {
public:
    KeyboardInputFactory(const std::shared_ptr<config::Config>& config);
    /** @brief Deleted destructor to prevent instantiation. */
    ~KeyboardInputFactory() override;

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent instantiation. */
    KeyboardInputFactory(const KeyboardInputFactory&) = delete;
    /** @brief Deleted copy assignment to prevent instantiation. */
    auto operator=(const KeyboardInputFactory&) -> KeyboardInputFactory& = delete;

    // Delete move operations
    /** @brief Deleted move constructor to prevent instantiation. */
    KeyboardInputFactory(KeyboardInputFactory&&) = delete;
    /** @brief Deleted move assignment to prevent instantiation. */
    auto operator=(KeyboardInputFactory&&) -> KeyboardInputFactory& = delete;

    /**
     * @brief Create an input handler for a specific command.
     * @param commandName Name of the command to create input for.
     * @return A unique pointer to the created input handler.
     *
     * Creates and returns an input handler configured for the specified command.
     * The configuration is loaded from the initialized configuration file.
     * @throws std::runtime_error if the factory is not initialized or the command
     * is not found in configuration.
     */
    [[nodiscard]] virtual auto createInput(const std::string& commandName) const -> std::unique_ptr<IInput> override;

    /**
     * @brief Check if a shortcut exists for a command.
     * @param commandName Name of the command to check.
     * @return true if a shortcut exists, false otherwise.
     *
     * Checks if there is a configured shortcut for the specified command.
     * @throws std::runtime_error if the factory is not initialized.
     */
    [[nodiscard]] virtual auto hasShortcut(const std::string& commandName) const -> bool override;

    /**
     * @brief Get a list of all configured commands.
     * @return Vector of command names that have shortcuts configured.
     *
     * Returns a list of all commands that have shortcuts configured in the
     * configuration file.
     * @throws std::runtime_error if the factory is not initialized.
     */
    [[nodiscard]] virtual auto getConfiguredCommands() const -> std::vector<std::string> override;

    /**
     * @brief Initialize the input factory with configuration.
     *
     * This method must be called before using any other methods of the factory.
     * It loads and validates the configuration file, creating a default one
     * if it doesn't exist.
     */
    virtual auto initialize() -> void override;

private:
    class KeyboardInputFactoryImpl;
#pragma warning(push)
#pragma warning(disable : 4251)
    std::unique_ptr<KeyboardInputFactoryImpl> pimpl_;
#pragma warning(pop)
};

}  // namespace palantir::input

#endif  // INPUT_FACTORY_HPP