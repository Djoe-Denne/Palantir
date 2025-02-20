#include "command/show_command.hpp"

#include "window/iwindow.hpp"
#include "window/window_manager.hpp"

namespace interview_cheater::command {
ShowCommand::ShowCommand(window::WindowManager& manager) : windowManager_(manager) {}

void ShowCommand::execute() {
    if (auto* window = windowManager_.getFirstWindow()) {
        window->show();
    }
}
}  // namespace interview_cheater::command
