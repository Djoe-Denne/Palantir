#include "command/toggle_window_anonymity_command.hpp"
#include "window/window_manager.hpp"
#include "window/iwindow.hpp"

namespace palantir::command {

auto ToggleWindowAnonymityCommand::execute() const -> void {
    auto windowManager = app_->getWindowManager();
    if (auto window = windowManager->getMainWindow()) {
        window->toggleWindowAnonymity();
    } else {
        throw std::runtime_error("No window found");
    }
}

auto ToggleWindowAnonymityCommand::useDebounce() const -> bool { return true; }

} // namespace palantir::command 