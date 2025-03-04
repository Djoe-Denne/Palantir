#include "command/show_command.hpp"
#include "window/window_manager.hpp"
#include "window/iwindow.hpp"

namespace palantir::command {

ShowCommand::ShowCommand() : app_(Application::getInstance()) {}  // NOLINT

auto ShowCommand::execute() -> void {
    auto windowManager = app_->getWindowManager();
    if (auto window = windowManager->getFirstWindow()) {
        window->show();
    } else {
        throw std::runtime_error("No window found");
    }
}

auto ShowCommand::useDebounce() -> bool { return true; }

} // namespace palantir::command 