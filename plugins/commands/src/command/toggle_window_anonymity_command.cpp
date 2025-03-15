#include "command/toggle_window_anonymity_command.hpp"
#include "window/window_manager.hpp"
#include "window/iwindow.hpp"

namespace palantir::command {

ToggleWindowAnonymityCommand::ToggleWindowAnonymityCommand() : app_(Application::getInstance()) {}  // NOLINT

auto ToggleWindowAnonymityCommand::execute() -> void {
    auto windowManager = app_->getWindowManager();
    if (auto window = windowManager->getMainWindow()) {
        window->toggleWindowAnonymity();
    } else {
        throw std::runtime_error("No window found");
    }
}

auto ToggleWindowAnonymityCommand::useDebounce() -> bool { return true; }

} // namespace palantir::command 