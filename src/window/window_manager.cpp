#include "window/window_manager.hpp"
#include <algorithm>

void WindowManager::addWindow(std::unique_ptr<IWindow> window) {
    windows.push_back(std::move(window));
}

void WindowManager::removeWindow(const IWindow* window) {
    auto it = std::find_if(windows.begin(), windows.end(),
                          [window](const auto& w) { return w.get() == window; });
    if (it != windows.end()) {
        windows.erase(it);
    }
}

IWindow* WindowManager::getFirstWindow() const {
    return windows.empty() ? nullptr : windows.front().get();
}

bool WindowManager::hasRunningWindows() const {
    return std::any_of(windows.begin(), windows.end(),
                      [](const auto& w) { return w->isRunning(); });
}

void WindowManager::executeCommand(std::unique_ptr<ICommand> command) {
    if (command) {
        command->execute();
    }
}