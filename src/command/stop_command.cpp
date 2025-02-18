#include "command/stop_command.hpp"

StopCommand::StopCommand(IWindow& window) : window_(window) {}

void StopCommand::execute() {
    window_.close();
}