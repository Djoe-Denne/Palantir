#pragma once

#include <functional>
#include <memory>
#include <string>

#include "core_export.hpp"

namespace palantir::command {

class ICommand;  // Forward declaration

/**
 * @class CommandFactory
 * @brief Factory class for managing and creating commands.
 *
 * This singleton factory class is responsible for registering and managing commands.
 * It maintains a map of command names to command instances and uses the PIMPL pattern
 * to hide implementation details.
 */
class PALANTIR_CORE_API CommandFactory {
public:
    using CommandCreator = std::function<std::unique_ptr<ICommand>()>;  // Function to create a command

    /** @brief Get the singleton instance of the factory. */
    static auto getInstance() -> std::shared_ptr<CommandFactory>;

    /** @brief Set the singleton instance of the factory. */
    static auto setInstance(const std::shared_ptr<CommandFactory>& instance) -> void;

    /** @brief Destructor. */
    virtual ~CommandFactory();

    // Delete copy operations
    CommandFactory(const CommandFactory&) = delete;
    auto operator=(const CommandFactory&) -> CommandFactory& = delete;

    // Delete move operations
    CommandFactory(CommandFactory&&) = delete;
    auto operator=(CommandFactory&&) -> CommandFactory& = delete;

    /**
     * @brief Register a command with the factory.
     * @param commandName Name of the command to register
     * @param creator Function to create the command
     */
    virtual auto registerCommand(const std::string& commandName, CommandCreator creator) -> void;

    /**
     * @brief Unregister a command from the factory.
     * @param commandName Name of the command to unregister
     * @return true if command was unregistered, false if not found
     */
    virtual auto unregisterCommand(const std::string& commandName) -> bool;

    /**
     * @brief Get a command from the factory.
     * @param name The name of the command to get.
     * @return Unique pointer to a new instance of the command if found, nullptr otherwise.
     */
    virtual auto getCommand(const std::string& name) -> std::unique_ptr<ICommand>;

protected:
    CommandFactory();

private:
    // Private implementation class forward declaration
    class CommandFactoryImpl;
    // Suppress C4251 warning for this specific line as Impl class is never accessed by client
#pragma warning(push)
#pragma warning(disable : 4251)
    std::unique_ptr<CommandFactoryImpl> pimpl_;
    static std::shared_ptr<CommandFactory> instance_;
#pragma warning(pop)
};

}  // namespace palantir::command
