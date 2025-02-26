#include "command/toggle_window_anonymity_command.hpp"
#include "window/window_manager.hpp"
#include "window/iwindow.hpp"

namespace interview_cheater::command {

ToggleWindowAnonymityCommand::ToggleWindowAnonymityCommand() : app_(Application::getInstance()) {}  // NOLINT

auto ToggleWindowAnonymityCommand::execute() -> void {
    auto windowManager = app_->getWindowManager();
    if (auto window = windowManager->getFirstWindow()) {
        window->toggleWindowAnonymity();
    } else {
        throw std::runtime_error("No window found");
    }
}

auto ToggleWindowAnonymityCommand::useDebounce() -> bool { return true; }

} // namespace interview_cheater::command 