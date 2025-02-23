#include "window/window_manager.hpp"

#include <algorithm>
#include <vector>

#include "command/icommand.hpp"
#include "window/iwindow.hpp"

namespace interview_cheater::window {

class WindowManager::WindowManagerImpl {
public:
    auto addWindow(std::unique_ptr<IWindow> window) -> void { 
        windows_.push_back(std::move(window)); 
    }

    auto removeWindow(const IWindow* window) -> void {
        auto iter = std::find_if(windows_.begin(), windows_.end(), 
            [window](const auto& win) { return win.get() == window; });
        if (iter != windows_.end()) {
            windows_.erase(iter);
        }
    }

    auto getFirstWindow() const -> IWindow* { 
        return windows_.empty() ? nullptr : windows_.front().get(); 
    }

    auto hasRunningWindows() const -> bool {
        return std::any_of(windows_.begin(), windows_.end(), 
            [](const auto& window) { return window->isRunning(); });
    }

    auto executeCommand(std::unique_ptr<interview_cheater::command::ICommand> command) -> void {
        if (command) {
            command->execute();
        }
    }

private:
    std::vector<std::unique_ptr<IWindow>> windows_{};
};

// Singleton implementation
WindowManager& WindowManager::getInstance() {
    static WindowManager instance;
    return instance;
}

// Constructor
WindowManager::WindowManager() : pimpl_(std::make_unique<WindowManagerImpl>()) {}

// Destructor
WindowManager::~WindowManager() = default;

// Delegate all public methods to the implementation
auto WindowManager::addWindow(std::unique_ptr<IWindow> window) -> void {
    pimpl_->addWindow(std::move(window));
}

auto WindowManager::removeWindow(const IWindow* window) -> void {
    pimpl_->removeWindow(window);
}

auto WindowManager::getFirstWindow() const -> IWindow* {
    return pimpl_->getFirstWindow();
}

auto WindowManager::hasRunningWindows() const -> bool {
    return pimpl_->hasRunningWindows();
}

auto WindowManager::executeCommand(std::unique_ptr<interview_cheater::command::ICommand> command) -> void {
    pimpl_->executeCommand(std::move(command));
}

}  // namespace interview_cheater::window