#include "window/window_manager.hpp"

#include <algorithm>

#include "command/icommand.hpp"
#include "window/iwindow.hpp"

namespace interview_cheater::window {
void WindowManager::addWindow(std::unique_ptr<IWindow> window) { windows_.push_back(std::move(window)); }

void WindowManager::removeWindow(const IWindow* window) {
    auto iter = std::find_if(windows_.begin(), windows_.end(), [window](const auto& win) { return win.get() == window; });
    if (iter != windows_.end()) {
        windows_.erase(iter);
    }
}

auto WindowManager::getFirstWindow() const -> IWindow* { return windows_.empty() ? nullptr : windows_.front().get(); }

auto WindowManager::hasRunningWindows() const -> bool {
    return std::any_of(windows_.begin(), windows_.end(), [](const auto& window) { return window->isRunning(); });
}

void WindowManager::executeCommand(std::unique_ptr<interview_cheater::command::ICommand> command) {
    if (command) {
        command->execute();
    }
}
}  // namespace interview_cheater::window