#include "command/toggle_transparency_command.hpp"
#include "window/window_manager.hpp"
#include "window/iwindow.hpp"

namespace palantir::command {
    
auto ToggleTransparencyCommand::execute() const -> void {
    auto windowManager = app_->getWindowManager();
    if (auto window = windowManager->getMainWindow()) {
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

auto ToggleTransparencyCommand::useDebounce() const -> bool { return true; }

} // namespace palantir::command 