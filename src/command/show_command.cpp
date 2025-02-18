#include "command/show_command.hpp"

ShowCommand::ShowCommand(IWindow& window) : window_(window) {}

void ShowCommand::execute() {
    window_.show();
}