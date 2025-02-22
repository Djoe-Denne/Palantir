#include "command/show_command.hpp"

#include "application.hpp"
#include "utils/auto_command_register.hpp"
#include "window/iwindow.hpp"

namespace interview_cheater::command {
ShowCommand::ShowCommand() : app_(*Application::getInstance()) {}  // NOLINT

auto ShowCommand::execute() -> void {
    if (auto* window = app_.getWindowManager().getFirstWindow()) {
        window->show();
    }
}

auto ShowCommand::useDebounce() -> bool { return true; }

}  // namespace interview_cheater::command

REGISTER_COMMAND("toggle", interview_cheater::command, ShowCommand)