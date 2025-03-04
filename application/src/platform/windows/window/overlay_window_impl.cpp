#include "window/overlay_window_impl.hpp"

#include <dwmapi.h>
#include <windows.h>

#include "utils/logger.hpp"
#include "window/component/content_manager.hpp"
#include "window/component/webview/webview.hpp"

namespace interview_cheater::window {
namespace {

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr BYTE WINDOW_ALPHA = 240;
constexpr int SQUARE_MARGIN_RATIO = 4;  // Divides width/height by this to get margin
constexpr COLORREF SQUARE_COLOR = RGB(255, 0, 0);

}  // namespace

OverlayWindow::Impl::Impl() // NOLINT
    : contentManager_(new component::ContentManager<component::webview::WebView>()) {}  

OverlayWindow::Impl::~Impl() {
    if (contentManager_) {
        contentManager_->destroy();
    }
    if (hwnd_ != nullptr) {
        DestroyWindow(hwnd_);
        hwnd_ = nullptr;
    }
}

LRESULT CALLBACK OverlayWindow::Impl::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {  // NOLINT
    Impl* window = nullptr;

    if (uMsg == WM_NCCREATE) {
        auto* create_struct = reinterpret_cast<LPCREATESTRUCT>(lParam);  // NOLINT
        window = static_cast<Impl*>(create_struct->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    } else {
        window = reinterpret_cast<Impl*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));  // NOLINT
    }

    if (window != nullptr) {
        return window->HandleMessage(hwnd, uMsg, wParam, lParam);
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

LRESULT OverlayWindow::Impl::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {  // NOLINT
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:
            if (contentManager_) {
                RECT bounds;
                GetClientRect(hwnd, &bounds);
                DEBUG_LOG("Resizing WebView2 to %ldx%ld", bounds.right - bounds.left, bounds.bottom - bounds.top);
                contentManager_->resize(bounds.right - bounds.left, bounds.bottom - bounds.top);
            }
            return 0;
        case WM_ERASEBKGND:
            return 1;  // Prevent background erasing to avoid flicker
        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
}

auto OverlayWindow::Impl::create() -> void {
    DEBUG_LOG("Starting overlay window creation");

    WNDCLASSEXW windowClass = {};
    windowClass.cbSize = sizeof(WNDCLASSEXW);
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = GetModuleHandleW(nullptr);
    windowClass.lpszClassName = L"InterviewCheaterClass";
    windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1); // NOLINT
    windowClass.style = CS_HREDRAW | CS_VREDRAW;

    if (RegisterClassExW(&windowClass) == 0) {
        DWORD error = GetLastError();
        DEBUG_LOG("Failed to register window class: %lu", error);
        throw std::runtime_error("Failed to register window class");
    }

    hwnd_ = CreateWindowExW(WS_EX_TOPMOST, L"InterviewCheaterClass", L"Interview Cheater", WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr,
                            GetModuleHandleW(nullptr), this);

    if (hwnd_ == nullptr) {
        DWORD error = GetLastError();
        DEBUG_LOG("Failed to create window: %lu", error);
        throw std::runtime_error("Failed to create window");
    }

    DEBUG_LOG("Window created successfully with handle: %p", hwnd_);

    try {
        // Show window immediately
        ShowWindow(hwnd_, SW_SHOW);
        UpdateWindow(hwnd_);
        DEBUG_LOG("Window shown successfully");

        // Initialize WebView2
        if (!contentManager_) {
            DEBUG_LOG("WebView2 instance is null");
            throw std::runtime_error("WebView2 instance is null");
        }

        DEBUG_LOG("Initializing WebView2");

        // Initialize WebView2 with completion callback
        contentManager_->initialize(hwnd_);

        // After WebView2 is initialized, set the window styles
        LONG_PTR exStyle = GetWindowLongPtrW(hwnd_, GWL_EXSTYLE);
        SetWindowLongPtrW(hwnd_, GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_TOOLWINDOW);  // NOLINT
        setTransparency(WINDOW_ALPHA);

        toggleWindowAnonymity();
        running_ = true;

        // Force a redraw of the window
        InvalidateRect(hwnd_, nullptr, TRUE);
        UpdateWindow(hwnd_);

        DEBUG_LOG("Window initialization completed successfully");
    } catch (const std::exception& e) {
        DEBUG_LOG("Exception during window initialization: %s", e.what());
        if (hwnd_ != nullptr) {
            DestroyWindow(hwnd_);
            hwnd_ = nullptr;
        }
        throw;
    }
}

auto OverlayWindow::Impl::show() -> void {
    if (hwnd_ == nullptr) {
        return;
    }

    if (IsWindowVisible(hwnd_) == TRUE) {
        ShowWindow(hwnd_, SW_HIDE);
    } else {
        ShowWindow(hwnd_, SW_SHOW);
        SetForegroundWindow(hwnd_);
        UpdateWindow(hwnd_);
    }
}

auto OverlayWindow::Impl::update() -> void {
    MSG msg;
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

auto OverlayWindow::Impl::close() -> void {
    if (contentManager_) {
        contentManager_->destroy();
    }
    if (hwnd_ != nullptr) {
        DestroyWindow(hwnd_);
        hwnd_ = nullptr;
    }
    running_ = false;
}

auto OverlayWindow::Impl::setTransparency(int transparency) -> void {
    if (hwnd_ != nullptr && transparency >= 0 && transparency <= 255) {  // NOLINT
        SetLayeredWindowAttributes(hwnd_, 0, transparency, LWA_ALPHA);
    }
}

auto OverlayWindow::Impl::toggleWindowAnonymity() -> void {
    if (hwnd_ != nullptr) {
        DWORD affinity = WDA_NONE;
        if (GetWindowDisplayAffinity(hwnd_, &affinity) ==
            TRUE) {  // If the window is displayable, set the display affinity
            DEBUG_LOG("Window display affinity:", affinity);
            if (affinity ==
                WDA_EXCLUDEFROMCAPTURE) {  // If the window is excluded from capture, set the display affinity to none
                DEBUG_LOG("Setting window display affinity to none");
                SetWindowDisplayAffinity(hwnd_, WDA_NONE);
                toggleWindowTool(false);
            } else {  // If the window is not excluded from capture, set the display affinity to exclude from capture
                DEBUG_LOG("Setting window display affinity to exclude from capture");
                SetWindowDisplayAffinity(hwnd_, WDA_EXCLUDEFROMCAPTURE);
                toggleWindowTool(true);
            }
        }
        DEBUG_LOG("Setting window position");
        SetWindowPos(hwnd_, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }
}

auto OverlayWindow::Impl::toggleWindowTool(bool isToolWindow) -> void {
    if (hwnd_ != nullptr) {
        LONG_PTR exStyle = GetWindowLongPtrW(hwnd_, GWL_EXSTYLE);
        if (exStyle & WS_EX_TOOLWINDOW &&  // NOLINT
            !isToolWindow) {               // If the window is a tool window, remove the tool window flag
            DEBUG_LOG("Removing tool window flag");
            exStyle &= ~WS_EX_TOOLWINDOW;            // NOLINT
        } else if (!(exStyle & WS_EX_TOOLWINDOW) &&  // NOLINT
                   isToolWindow) {                   // If the window is not a tool window, add the tool window flag
            DEBUG_LOG("Adding tool window flag");
            exStyle |= WS_EX_TOOLWINDOW;  // NOLINT
        }
        DEBUG_LOG("Setting window extended style to:", exStyle);
        SetWindowLongPtrW(hwnd_, GWL_EXSTYLE, exStyle);
    }
}

auto OverlayWindow::Impl::getNativeHandle() const -> void* { return hwnd_; }

auto OverlayWindow::Impl::isRunning() const -> bool { return running_; }

auto OverlayWindow::Impl::setRunning(bool state) -> void { running_ = state; }

auto OverlayWindow::Impl::getContentManager() const -> std::shared_ptr<component::IContentManager> {
    return contentManager_;
}
}  // namespace interview_cheater::window
