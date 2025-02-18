#include "window/window_manager.hpp"

void WindowManager::addWindow(std::unique_ptr<IWindow> window) {
    windows_.push_back(std::move(window));
}

IWindow* WindowManager::getFirstWindow() const {
    if (!windows_.empty()) {
        return windows_.front().get();
    }
    return nullptr;
}

bool WindowManager::hasRunningWindows() const {
    for (const auto& window : windows_) {
        if (window->isRunning()) {
            return true;
        }
    }
    return false;
}

void WindowManager::executeCommand(std::unique_ptr<ICommand> command) {
    command->execute();
}