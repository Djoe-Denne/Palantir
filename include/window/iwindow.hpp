#ifndef IWINDOW_HPP
#define IWINDOW_HPP

#include <memory>

namespace interview_cheater::window {

class IWindow {
public:
    virtual ~IWindow() = default;

    // Delete copy operations
    IWindow(const IWindow&) = delete;
    auto operator=(const IWindow&) -> IWindow& = delete;

    // Define move operations
    IWindow(IWindow&&) noexcept = default;
    auto operator=(IWindow&&) noexcept -> IWindow& = default;

    // Platform-agnostic window lifecycle methods
    virtual auto create() -> void = 0;
    virtual auto show() -> void = 0;
    virtual auto update() -> void = 0;
    virtual auto close() -> void = 0;

    // Window state methods
    [[nodiscard]] virtual auto isRunning() const -> bool = 0;
    virtual auto setRunning(bool runningState) -> void = 0;

    // Native handle accessor - returns void* to avoid platform-specific types
    // The actual implementation will cast this to the appropriate type
    [[nodiscard]] virtual auto getNativeHandle() const -> void* = 0;

protected:
    IWindow() = default;
};

}  // namespace interview_cheater::window

#endif  // IWINDOW_HPP