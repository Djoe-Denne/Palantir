#include "command/show_command.hpp"

#include "window/iwindow.hpp"
#include "window/window_manager.hpp"

namespace interview_cheater::command {
ShowCommand::ShowCommand(window::WindowManager& manager) : manager_(manager) {}

auto ShowCommand::execute() -> void {
    if (auto* window = manager_.getFirstWindow()) {
        window->show();
    }
}

auto ShowCommand::getName() const -> const std::string& { return name_; }
}  // namespace interview_cheater::command
