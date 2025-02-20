#pragma once

#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

namespace interview_cheater::command
{
class ICommand
{
public:
    virtual ~ICommand()    = default;
    virtual void execute() = 0;
};
}  // namespace interview_cheater::command
#endif  // ICOMMAND_HPP