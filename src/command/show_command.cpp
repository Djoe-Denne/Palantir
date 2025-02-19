#include "command/show_command.hpp"

using namespace interview_cheater;
command::ShowCommand::ShowCommand(window::IWindow& window) : window_(window) {}

void command::ShowCommand::execute() {
    window_.show();
}