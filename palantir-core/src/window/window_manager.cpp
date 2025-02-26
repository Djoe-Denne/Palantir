#include "window/window_manager.hpp"

#include <algorithm>
#include <vector>

#include "command/icommand.hpp"
#include "window/iwindow.hpp"

namespace interview_cheater::window {

// Initialize the static instance
std::shared_ptr<WindowManager> WindowManager::instance_;

class WindowManager::WindowManagerImpl {
public:
    auto addWindow(std::shared_ptr<IWindow> window) -> void { windows_.push_back(window); }

    auto removeWindow(const IWindow* window) -> void {
        auto iter =
            std::find_if(windows_.begin(), windows_.end(), [window](const auto& win) { return win.get() == window; });
        if (iter != windows_.end()) {
            windows_.erase(iter);
        }
    }

    auto getFirstWindow() const -> std::shared_ptr<IWindow> { return windows_.empty() ? nullptr : windows_.front(); }

    auto hasRunningWindows() const -> bool {
        return std::any_of(windows_.begin(), windows_.end(), [](const auto& window) { return window->isRunning(); });
    }

    auto executeCommand(std::unique_ptr<interview_cheater::command::ICommand> command) -> void {
        if (command) {
            command->execute();
        }
    }

private:
    std::vector<std::shared_ptr<IWindow>> windows_{};
};

// Singleton implementation
auto WindowManager::getInstance() -> std::shared_ptr<WindowManager> {
    if (!instance_) {
        // Create a new instance using make_shared
        instance_ = std::shared_ptr<WindowManager>(new WindowManager());
    }
    return instance_;
}

auto WindowManager::setInstance(const std::shared_ptr<WindowManager>& instance) -> void { instance_ = instance; }

// Constructor
WindowManager::WindowManager() : pimpl_(std::make_unique<WindowManagerImpl>()) {}

// Destructor
WindowManager::~WindowManager() = default;

// Delegate all public methods to the implementation
auto WindowManager::addWindow(std::unique_ptr<IWindow> window) -> void { pimpl_->addWindow(std::move(window)); }

auto WindowManager::removeWindow(const IWindow* window) -> void { pimpl_->removeWindow(window); }

auto WindowManager::getFirstWindow() const -> std::shared_ptr<IWindow> {
    return pimpl_->getFirstWindow();
}

auto WindowManager::hasRunningWindows() const -> bool { return pimpl_->hasRunningWindows(); }

auto WindowManager::executeCommand(std::unique_ptr<interview_cheater::command::ICommand> command) -> void {
    pimpl_->executeCommand(std::move(command));
}

}  // namespace interview_cheater::window