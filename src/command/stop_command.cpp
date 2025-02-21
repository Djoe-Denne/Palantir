#include "command/stop_command.hpp"

#include "application.hpp"

namespace interview_cheater::command {

StopCommand::StopCommand(Application& app) : app_(app) {}

auto StopCommand::execute() -> void { app_.quit(); }

auto StopCommand::getName() const -> const std::string& { return name_; }

}  // namespace interview_cheater::command