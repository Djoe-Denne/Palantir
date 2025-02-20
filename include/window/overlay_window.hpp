#ifndef OVERLAY_WINDOW_HPP
#define OVERLAY_WINDOW_HPP

#include <memory>

#include "window/iwindow.hpp"

namespace interview_cheater::window {

class OverlayWindow : public IWindow {
public:
    OverlayWindow();
    ~OverlayWindow() override;

    // Delete copy operations
    OverlayWindow(const OverlayWindow&) = delete;
    auto operator=(const OverlayWindow&) -> OverlayWindow& = delete;

    // Define move operations
    OverlayWindow(OverlayWindow&&) noexcept = default;
    auto operator=(OverlayWindow&&) noexcept -> OverlayWindow& = default;

    auto create() -> void override;
    auto show() -> void override;
    auto update() -> void override;
    auto close() -> void override;
    [[nodiscard]] auto getNativeHandle() const -> void* override;

    [[nodiscard]] auto isRunning() const -> bool override;
    auto setRunning(bool runningState) -> void override;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl_;
    bool running_{false};
};

}  // namespace interview_cheater::window

#endif  // OVERLAY_WINDOW_HPP