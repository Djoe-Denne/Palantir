#pragma once

#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

namespace interview_cheater::command {
class ICommand {
public:
    virtual ~ICommand() = default;
    ICommand(const ICommand&) = delete;
    auto operator=(const ICommand&) -> ICommand& = delete;
    ICommand(ICommand&&) = delete;
    auto operator=(ICommand&&) -> ICommand& = delete;

    virtual void execute() = 0;

protected:
    ICommand() = default;
};
}  // namespace interview_cheater::command
#endif  // ICOMMAND_HPP