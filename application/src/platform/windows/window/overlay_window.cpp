#include "window/overlay_window.hpp"

#include <dwmapi.h>
#include <windows.h>

#include "utils/logger.hpp"

namespace interview_cheater::window {
namespace {

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr BYTE WINDOW_ALPHA = 240;
constexpr int SQUARE_MARGIN_RATIO = 4;  // Divides width/height by this to get margin
constexpr COLORREF SQUARE_COLOR = RGB(255, 0, 0);

auto CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT {
    switch (uMsg) {
        return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT paintStruct;
            HDC hdc = BeginPaint(hwnd, &paintStruct);
            RECT rect;
            GetClientRect(hwnd, &rect);
            const int width = rect.right;
            const int height = rect.bottom;
            HBRUSH brush = CreateSolidBrush(SQUARE_COLOR);
            const RECT square = {width / SQUARE_MARGIN_RATIO, height / SQUARE_MARGIN_RATIO,
                                 (SQUARE_MARGIN_RATIO - 1) * width / SQUARE_MARGIN_RATIO,
                                 (SQUARE_MARGIN_RATIO - 1) * height / SQUARE_MARGIN_RATIO};
            FillRect(hdc, &square, brush);
            DeleteObject(brush);
            EndPaint(hwnd, &paintStruct);
        }
            return 0;
        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
}
}  // namespace

class OverlayWindow::Impl {
public:
    Impl() = default;

    auto create() -> void {
        WNDCLASSEXW windowClass = {};
        windowClass.cbSize = sizeof(WNDCLASSEXW);
        windowClass.lpfnWndProc = WindowProc;
        windowClass.hInstance = GetModuleHandleW(nullptr);
        windowClass.lpszClassName = L"InterviewCheaterClass";

        RegisterClassExW(&windowClass);

        hwnd_ = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW, L"InterviewCheaterClass",
                                L"Interview Cheater", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
                                WINDOW_HEIGHT, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);

        if (hwnd_ != nullptr) {
            // Store the this pointer
            SetWindowLongPtr(hwnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

            // Make the window semi-transparent
            setTransparency(WINDOW_ALPHA);
            toggleWindowAnonymity();

            // Hide initially
            ShowWindow(hwnd_, SW_HIDE);
            running_ = true;
        }
    }

    auto show() -> void {
        if (hwnd_ == nullptr) {
            return;
        }

        if (IsWindowVisible(hwnd_) == TRUE) {
            ShowWindow(hwnd_, SW_HIDE);
        } else {
            ShowWindow(hwnd_, SW_SHOW);
            SetForegroundWindow(hwnd_);
            UpdateWindow(hwnd_);  // Ensure the window is updated
        }
    }

    auto update() -> void {  // NOLINT(readability-convert-member-functions-to-static)
        MSG msg;
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    auto close() -> void {
        if (hwnd_ != nullptr) {
            DestroyWindow(hwnd_);
            hwnd_ = nullptr;
        }
        running_ = false;
    }

    auto setTransparency(int transparency) -> void {
        if (hwnd_ != nullptr && transparency >= 0 && transparency <= 255) {  // NOLINT(readability-magic-numbers)
            SetLayeredWindowAttributes(hwnd_, 0, transparency, LWA_ALPHA);
        }
    }

    auto toggleWindowAnonymity() -> void {
        if (hwnd_ != nullptr) {
            // Toggle screen capture exclusion
            DWORD affinity = WDA_NONE;
            if (GetWindowDisplayAffinity(hwnd_, &affinity) == TRUE) {
                if (affinity == WDA_EXCLUDEFROMCAPTURE) {
                    SetWindowDisplayAffinity(hwnd_, WDA_NONE);
                } else {
                    SetWindowDisplayAffinity(hwnd_, WDA_EXCLUDEFROMCAPTURE);
                }
            }

            // Toggle window styles
            LONG_PTR exStyle = GetWindowLongPtrW(hwnd_, GWL_EXSTYLE);
            if (exStyle & WS_EX_TOOLWINDOW) {
                // Remove stealth mode
                exStyle &= ~WS_EX_TOOLWINDOW;
            } else {
                // Enable stealth mode
                exStyle |= WS_EX_TOOLWINDOW;
            }
            SetWindowLongPtrW(hwnd_, GWL_EXSTYLE, exStyle);

            // Force window to update its appearance
            SetWindowPos(hwnd_, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
        }
    }

    [[nodiscard]] auto getNativeHandle() const -> void* { return hwnd_; }

    [[nodiscard]] auto isRunning() const -> bool { return running_; }

    auto setRunning(bool state) -> void { running_ = state; }

private:
    HWND hwnd_{nullptr};
    bool running_{false};
};

OverlayWindow::OverlayWindow() : pImpl_(std::make_unique<Impl>()) {}

OverlayWindow::~OverlayWindow() = default;

auto OverlayWindow::create() -> void { pImpl_->create(); }
auto OverlayWindow::show() -> void { pImpl_->show(); }
auto OverlayWindow::update() -> void { pImpl_->update(); }
auto OverlayWindow::close() -> void { pImpl_->close(); }
auto OverlayWindow::setTransparency(int transparency) -> void { pImpl_->setTransparency(transparency); }
auto OverlayWindow::toggleWindowAnonymity() -> void { pImpl_->toggleWindowAnonymity(); }
auto OverlayWindow::getNativeHandle() const -> void* { return pImpl_->getNativeHandle(); }
auto OverlayWindow::isRunning() const -> bool { return pImpl_->isRunning(); }
auto OverlayWindow::setRunning(bool runningState) -> void { pImpl_->setRunning(runningState); }

}  // namespace interview_cheater::window