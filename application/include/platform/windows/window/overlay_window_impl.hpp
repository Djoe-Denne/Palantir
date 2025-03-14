#ifndef OVERLAY_WINDOW_IMPL_HPP
#define OVERLAY_WINDOW_IMPL_HPP

#include <windows.h>

#include <memory>

#include "window/component/content_manager.hpp"
#include "window/component/icontent_size_observer.hpp"
#include "window/component/webview/webview.hpp"
#include "window/overlay_window.hpp"

namespace palantir::window {

class OverlayWindow::Impl {
public:
    Impl();
    ~Impl();

    Impl(const Impl&) = delete;
    auto operator=(const Impl&) -> Impl& = delete;
    Impl(Impl&&) noexcept = delete;
    auto operator=(Impl&&) noexcept -> Impl& = delete;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);  // NOLINT
    LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);               // NOLINT
    auto create() -> void;
    auto show() -> void;
    auto update() -> void;
    auto close() -> void;
    auto setTransparency(int transparency) -> void;
    auto toggleWindowAnonymity() -> void;
    [[nodiscard]] auto getNativeHandle() const -> void*;
    [[nodiscard]] auto isRunning() const -> bool;
    auto setRunning(bool state) -> void;
    [[nodiscard]] auto getContentManager() const -> std::shared_ptr<component::IContentManager>;

    auto toggleWindowTool(bool isToolWindow) -> void;
    auto updateWindowSize(int contentWidth, int contentHeight) -> void;
    auto getCurrentScreenResolution() -> std::pair<int, int>;
    auto makeWindowFrameless() -> void;

private:
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr BYTE WINDOW_ALPHA = 240;
    static constexpr COLORREF TRANSPARENT_COLOR = 0x00FFFFFF;  // Transparent
    static constexpr int BORDER_PADDING = 0;                   // Padding around content
    static constexpr int RIGHT_FRAME_WIDTH = 0;                // Width of the right frame

    HWND hwnd_{nullptr};
    bool running_{false};
    std::shared_ptr<component::ContentManager<component::webview::WebView>> contentManager_;
    int currentWidth_{WINDOW_WIDTH};
    int currentHeight_{WINDOW_HEIGHT};
};

}  // namespace palantir::window

#endif  // OVERLAY_WINDOW_IMPL_HPP