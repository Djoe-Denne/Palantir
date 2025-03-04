#include "command/stop_command.hpp"

namespace palantir::command {

StopCommand::StopCommand() : app_(Application::getInstance()) {}  // NOLINT

auto StopCommand::execute() -> void { app_->quit(); }

auto StopCommand::useDebounce() -> bool { return false; }

} // namespace palantir::command 