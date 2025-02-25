#include "command/show_command.hpp"
#include "window/window_manager.hpp"
#include "window/iwindow.hpp"

namespace interview_cheater::command {

ShowCommand::ShowCommand() : app_(*Application::getInstance()) {}  // NOLINT

auto ShowCommand::execute() -> void {
    app_.getWindowManager().getFirstWindow()->show();
}

auto ShowCommand::useDebounce() -> bool { return true; }

} // namespace interview_cheater::command 