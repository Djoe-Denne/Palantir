#include "command/show_command.hpp"

#include "window/iwindow.hpp"

namespace interview_cheater::command
{
ShowCommand::ShowCommand(window::IWindow& window) : window_(window) {}

void ShowCommand::execute()
{
    window_.show();
}
}  // namespace interview_cheater::command
