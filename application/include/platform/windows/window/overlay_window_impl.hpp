#ifndef OVERLAY_WINDOW_IMPL_HPP
#define OVERLAY_WINDOW_IMPL_HPP

#include <windows.h>
#include <memory>
#include "window/overlay_window.hpp"
#include "window/component/content_manager.hpp"
#include "window/component/webview/webview.hpp"

namespace interview_cheater::window {

class OverlayWindow::Impl {
public:
    Impl();
    ~Impl();

    Impl(const Impl&) = delete;
    auto operator=(const Impl&) -> Impl& = delete;
    Impl(Impl&&) noexcept = delete;
    auto operator=(Impl&&) noexcept -> Impl& = delete;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // NOLINT
    LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // NOLINT
    auto create() -> void;
    auto show() -> void;
    auto update() -> void;
    auto close() -> void;
    auto setTransparency(int transparency) -> void;
    auto toggleWindowAnonymity() -> void;
    [[nodiscard]] auto getNativeHandle() const -> void*;
    [[nodiscard]] auto isRunning() const -> bool;
    auto setRunning(bool state) -> void;

private:
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr BYTE WINDOW_ALPHA = 240;

    auto toggleWindowTool(bool isToolWindow) -> void;

    HWND hwnd_{nullptr};
    bool running_{false};
    std::unique_ptr<component::ContentManager<component::webview::WebView>> contentManager_;
};

} // namespace interview_cheater::window

#endif // OVERLAY_WINDOW_IMPL_HPP 