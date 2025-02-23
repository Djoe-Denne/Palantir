#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "core_export.hpp"

namespace interview_cheater::command {


class ICommand;  // Forward declaration

/**
 * @class CommandFactory 
 * @brief Factory class for managing and creating commands.
 *
 * This static factory class is responsible for registering and managing commands.
 * It maintains a map of command names to command instances. The class is 
 * non-instantiable and provides only static methods.
 */
class PALANTIR_CORE_API CommandFactory {
public:
    using CommandCreator = std::unique_ptr<ICommand>(*)(); // Function pointer to create a command

    /** @brief Deleted constructor to prevent instantiation. */
    CommandFactory() = delete;
    /** @brief Deleted destructor to prevent instantiation. */
    ~CommandFactory() = delete;

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent instantiation. */
    CommandFactory(const CommandFactory&) = delete;
    /** @brief Deleted copy assignment to prevent instantiation. */
    auto operator=(const CommandFactory&) -> CommandFactory& = delete;

    // Delete move operations
    /** @brief Deleted move constructor to prevent instantiation. */
    CommandFactory(CommandFactory&&) = delete;
    /** @brief Deleted move assignment to prevent instantiation. */
    auto operator=(CommandFactory&&) -> CommandFactory& = delete;

    /**
     * @brief Register a command with the factory.
     * @param command Unique pointer to the command to register.
     * 
     * Registers a command in the factory's command map using the command's name
     * as the key. Takes ownership of the provided command.
     */
    static auto registerCommand(const std::string& commandName, CommandCreator creator) -> void;

    /**
     * @brief Get a command from the factory.
     * @param name The name of the command to get.
     * @return Const pointer to the command if found, nullptr otherwise.
     */
    static auto getCommand(const std::string& name) -> std::unique_ptr<ICommand>;

private:
    static std::unordered_map<std::string, CommandCreator> commands_;  ///< Map of registered commands
};


}  // namespace interview_cheater::command

