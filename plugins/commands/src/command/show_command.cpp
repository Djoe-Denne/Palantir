#include "command/show_command.hpp"
#include "window/window_manager.hpp"
#include "window/iwindow.hpp"

namespace palantir::command {

auto ShowCommand::execute() const -> void {
    auto windowManager = app_->getWindowManager();
    if (auto window = windowManager->getMainWindow()) {
        window->show();
    } else {
        throw std::runtime_error("No window found");
    }
}

auto ShowCommand::useDebounce() const -> bool { return true; }

} // namespace palantir::command 