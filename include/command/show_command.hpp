#ifndef SHOW_COMMAND_HPP
#define SHOW_COMMAND_HPP

#include "command/icommand.hpp"

namespace interview_cheater
{
namespace window
{
class IWindow;  // Forward declaration
}

namespace command
{
class ShowCommand : public ICommand
{
public:
    explicit ShowCommand(window::IWindow& window);
    void execute() override;

private:
    window::IWindow& window_;
};
}  // namespace command
}  // namespace interview_cheater
#endif  // SHOW_COMMAND_HPP