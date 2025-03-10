#include "command/toggle_transparency_command.hpp"
#include "window/window_manager.hpp"
#include "window/iwindow.hpp"

namespace palantir::command {

ToggleTransparencyCommand::ToggleTransparencyCommand() : app_(Application::getInstance()) {}  // NOLINT

auto ToggleTransparencyCommand::execute() -> void {
    auto windowManager = app_->getWindowManager();
    if (auto window = windowManager->getFirstWindow()) {
        int transparency = getTransparency();
        if (transparency == 100) {
            window->setTransparency(255);
        } else {
            window->setTransparency(100);
        }
    } else {
        throw std::runtime_error("No window found");
    }
}

auto ToggleTransparencyCommand::useDebounce() -> bool { return true; }

} // namespace palantir::command 