#include "command/stop_command.hpp"

namespace interview_cheater {
namespace command {

StopCommand::StopCommand() : app_(*Application::getInstance()) {}  // NOLINT

auto StopCommand::execute() -> void { app_.quit(); }

auto StopCommand::useDebounce() -> bool { return false; }

} // namespace command
} // namespace interview_cheater 