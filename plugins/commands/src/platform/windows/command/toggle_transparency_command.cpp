#include "command/toggle_transparency_command.hpp"

#include <windows.h>

#include "window/window_manager.hpp"
#include "window/iwindow.hpp"

namespace palantir::command {

auto ToggleTransparencyCommand::getTransparency() -> int {
    auto windowManager = app_->getWindowManager();
    if (auto window = windowManager->getMainWindow()) {
        auto hwnd = reinterpret_cast<HWND>(window->getNativeHandle());
        if (hwnd != nullptr) {
            BYTE transparency;
            if (GetLayeredWindowAttributes(hwnd, 0, &transparency, nullptr) == TRUE) {
                return transparency;
            }
        }
    }
    return 0;
}
} // namespace palantir::command 