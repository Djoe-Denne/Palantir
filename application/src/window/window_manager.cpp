#include "window/window_manager.hpp"

#include <algorithm>

#include "command/icommand.hpp"
#include "window/iwindow.hpp"
namespace interview_cheater::window {
auto WindowManager::addWindow(std::unique_ptr<IWindow> window) -> void { windows_.push_back(std::move(window)); }

auto WindowManager::removeWindow(const IWindow* window) -> void {
    auto iter =
        std::find_if(windows_.begin(), windows_.end(), [window](const auto& win) { return win.get() == window; });
    if (iter != windows_.end()) {
        windows_.erase(iter);
    }
}

auto WindowManager::getFirstWindow() const -> IWindow* { return windows_.empty() ? nullptr : windows_.front().get(); }

auto WindowManager::hasRunningWindows() const -> bool {
    return std::any_of(windows_.begin(), windows_.end(), [](const auto& window) { return window->isRunning(); });
}

auto WindowManager::executeCommand(std::unique_ptr<interview_cheater::command::ICommand> command) -> void {
    if (command) {
        command->execute();
    }
}
}  // namespace interview_cheater::window