#pragma once

#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

namespace interview_cheater::command {
class ICommand {
public:
    virtual ~ICommand() = default;

    // Delete copy operations
    ICommand(const ICommand&) = delete;
    auto operator=(const ICommand&) -> ICommand& = delete;

    // Define move operations
    ICommand(ICommand&&) noexcept = default;
    auto operator=(ICommand&&) noexcept -> ICommand& = default;

    virtual auto execute() -> void = 0;

protected:
    ICommand() = default;
};
}  // namespace interview_cheater::command
#endif  // ICOMMAND_HPP