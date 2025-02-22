/**
 * @file icommand.hpp
 * @brief Defines the command interface for the command pattern implementation.
 *
 * This file contains the ICommand interface which serves as the base for all commands
 * in the application. It follows the Command pattern to encapsulate actions that can
 * be triggered by various inputs and signals.
 */

#pragma once

#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include <string>

namespace interview_cheater::command {

/**
 * @class ICommand
 * @brief Interface for the Command pattern implementation.
 *
 * Provides a common interface for all commands in the application.
 * Commands encapsulate actions that can be executed in response to
 * user inputs or system events. The interface supports move semantics
 * but prohibits copying to ensure unique ownership of resources.
 */
class ICommand {
public:
    /** @brief Virtual destructor to ensure proper cleanup of derived classes. */
    virtual ~ICommand() = default;

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent command duplication. */
    ICommand(const ICommand&) = delete;
    /** @brief Deleted copy assignment to prevent command duplication. */
    auto operator=(const ICommand&) -> ICommand& = delete;

    // Define move operations
    /** @brief Default move constructor for transfer of command ownership. */
    ICommand(ICommand&&) noexcept = default;
    /** @brief Default move assignment for transfer of command ownership. */
    auto operator=(ICommand&&) noexcept -> ICommand& = default;

    /**
     * @brief Execute the command's action.
     *
     * This method should be implemented by concrete commands to define
     * their specific behavior when executed.
     */
    virtual auto execute() -> void = 0;

    /** @brief Whether the command should use debounce. */
    virtual auto useDebounce() -> bool = 0;

protected:
    /** @brief Protected default constructor to prevent direct instantiation. */
    ICommand() = default;
};
}  // namespace interview_cheater::command
#endif  // ICOMMAND_HPP