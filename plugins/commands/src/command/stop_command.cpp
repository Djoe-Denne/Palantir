#include "command/stop_command.hpp"

namespace palantir::command {

auto StopCommand::execute() const -> void { app_->quit(); }

auto StopCommand::useDebounce() const -> bool { return false; }

} // namespace palantir::command 