/**
 * @file input_factory.hpp
 * @brief Defines the factory for creating input handlers.
 *
 * This file contains the InputFactory class which is responsible for creating
 * input handlers based on configuration. It manages the initialization of the
 * input system and creation of input handlers for different commands.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include "input/iinput.hpp"
#include "core_export.hpp"

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
class PALANTIR_CORE_API IInputFactory {
public:
    /** @brief Deleted destructor to prevent instantiation. */
    virtual ~IInputFactory() = default;

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent instantiation. */
    IInputFactory(const IInputFactory&) = delete;
    /** @brief Deleted copy assignment to prevent instantiation. */
    auto operator=(const IInputFactory&) -> IInputFactory& = delete;

    // Delete move operations
    /** @brief Deleted move constructor to prevent instantiation. */
    IInputFactory(IInputFactory&&) = delete;
    /** @brief Deleted move assignment to prevent instantiation. */
    auto operator=(IInputFactory&&) -> IInputFactory& = delete;

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
    [[nodiscard]] virtual auto createInput(const std::string& commandName) const -> std::unique_ptr<IInput> = 0;

    /**
     * @brief Check if a shortcut exists for a command.
     * @param commandName Name of the command to check.
     * @return true if a shortcut exists, false otherwise.
     *
     * Checks if there is a configured shortcut for the specified command.
     * @throws std::runtime_error if the factory is not initialized.
     */
    [[nodiscard]] virtual auto hasShortcut(const std::string& commandName) const -> bool = 0;

    /**
     * @brief Get a list of all configured commands.
     * @return Vector of command names that have shortcuts configured.
     *
     * Returns a list of all commands that have shortcuts configured in the
     * configuration file.
     * @throws std::runtime_error if the factory is not initialized.
     */
    [[nodiscard]] virtual auto getConfiguredCommands() const -> std::vector<std::string> = 0;

    /**
     * @brief Initialize the input factory with configuration.
     *
     * This method must be called before using any other methods of the factory.
     * It loads and validates the configuration file, creating a default one
     * if it doesn't exist.
     */
    virtual auto initialize() -> void = 0;

protected:
    /** @brief Protected default constructor to prevent direct instantiation. */
    IInputFactory() = default;
};

}  // namespace palantir::input
