#include "command/stop_command.hpp"

#include "platform/application.hpp"
#include "utils/auto_command_register.hpp"

namespace interview_cheater::command {

StopCommand::StopCommand() : app_(*Application::getInstance()) {}  // NOLINT

auto StopCommand::execute() -> void { app_.quit(); }

auto StopCommand::useDebounce() -> bool { return false; }

}  // namespace interview_cheater::command

REGISTER_COMMAND("stop", interview_cheater::command, StopCommand)
