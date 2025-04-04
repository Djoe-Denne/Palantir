#include "window/window_manager.hpp"

#include <algorithm>
#include <vector>

#include "command/icommand.hpp"
#include "window/iwindow.hpp"

namespace palantir::window {

// Initialize the static instance
std::shared_ptr<WindowManager> WindowManager::instance_;

class WindowManager::WindowManagerImpl {
public:
    auto addWindow(const std::shared_ptr<IWindow>& window) -> void { windows_.push_back(window); }

    auto removeWindow(const IWindow* window) -> void {
        auto iter =
            std::find_if(windows_.begin(), windows_.end(), [window](const auto& win) { return win.get() == window; });
        if (iter != windows_.end()) {
            windows_.erase(iter);
        }
    }

    [[nodiscard]] auto getMainWindow() const -> std::shared_ptr<IWindow> { return getWindowByType(WindowType::MAIN); }

    [[nodiscard]] auto getWindowByType(WindowType type) const -> std::shared_ptr<IWindow> {
        auto iterator = std::find_if(windows_.begin(), windows_.end(),
                                     [type](const auto& window) { return window->getWindowType() == type; });
        return (iterator != windows_.end()) ? *iterator : nullptr;
    }

    [[nodiscard]] auto hasRunningWindows() const -> bool {
        return std::any_of(windows_.begin(), windows_.end(), [](const auto& window) { return window->isRunning(); });
    }

    auto executeCommand(std::unique_ptr<palantir::command::ICommand> command) const -> void {
        if (command) {
            command->execute();
        }
    }

private:
    std::vector<std::shared_ptr<IWindow>> windows_;
};

// Singleton implementation
auto WindowManager::getInstance() -> const std::shared_ptr<WindowManager>& {
    if (!instance_) {
        // Create a new instance using make_shared
        instance_ = std::shared_ptr<WindowManager>(new WindowManager());
    }
    return instance_;
}

auto WindowManager::setInstance(const std::shared_ptr<WindowManager>& instance) -> void { instance_ = instance; }

// Constructor
WindowManager::WindowManager() : pimpl_(std::make_unique<WindowManagerImpl>()) {}  // NOLINT

// Destructor
WindowManager::~WindowManager() = default;

// Delegate all public methods to the implementation
auto WindowManager::addWindow(const std::shared_ptr<IWindow>& window) -> void { pimpl_->addWindow(window); }

auto WindowManager::removeWindow(const IWindow* window) -> void { pimpl_->removeWindow(window); }

auto WindowManager::getMainWindow() const -> std::shared_ptr<IWindow> { return pimpl_->getMainWindow(); }

auto WindowManager::hasRunningWindows() const -> bool { return pimpl_->hasRunningWindows(); }

auto WindowManager::executeCommand(std::unique_ptr<palantir::command::ICommand> command) const -> void {
    pimpl_->executeCommand(std::move(command));
}

}  // namespace palantir::window