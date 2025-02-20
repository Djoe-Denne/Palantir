#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP

#include <memory>
#include <vector>

namespace interview_cheater {
namespace command {
// Forward declaration for ICommand
class ICommand;
}  // namespace command

namespace window {
// Forward declaration for IWindow
class IWindow;

class WindowManager {
public:
    WindowManager() = default;
    ~WindowManager() = default;

    // Delete copy operations
    WindowManager(const WindowManager&) = delete;
    auto operator=(const WindowManager&) -> WindowManager& = delete;

    // Define move operations
    WindowManager(WindowManager&&) noexcept = default;
    auto operator=(WindowManager&&) noexcept -> WindowManager& = default;

    auto addWindow(std::unique_ptr<IWindow> window) -> void;
    auto removeWindow(const IWindow* window) -> void;
    [[nodiscard]] auto getFirstWindow() const -> IWindow*;
    [[nodiscard]] auto hasRunningWindows() const -> bool;
    auto executeCommand(std::unique_ptr<interview_cheater::command::ICommand> command) -> void;

private:
    std::vector<std::unique_ptr<IWindow>> windows_;
};
}  // namespace window
}  // namespace interview_cheater

#endif  // WINDOW_MANAGER_HPP