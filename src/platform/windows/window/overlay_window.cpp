#include "window/overlay_window.hpp"

#include <dwmapi.h>
#include <windows.h>

#include "utils/logger.hpp"

namespace interview_cheater::window {
namespace {
auto CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT {
    auto* window =
        reinterpret_cast<OverlayWindow*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));  // NOLINT(performance-no-int-to-ptr)

    switch (uMsg) {
        case WM_CLOSE:
            if (window != nullptr) {
                window->setRunning(false);
            }
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            if (window != nullptr) {
                PAINTSTRUCT paintStruct;
                HDC hdc = BeginPaint(hwnd, &paintStruct);
                RECT rect;
                GetClientRect(hwnd, &rect);
                int width = rect.right;
                int height = rect.bottom;
                HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
                RECT square = {width / 4, height / 4, 3 * width / 4, 3 * height / 4};
                FillRect(hdc, &square, brush);
                DeleteObject(brush);
                EndPaint(hwnd, &paintStruct);
                return 0;
            }
            return 0;
        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr BYTE WINDOW_ALPHA = 240;
}  // namespace

class OverlayWindow::Impl {
public:
    Impl() = default;

    void create() {
        WNDCLASSEXW windowClass = {};
        windowClass.cbSize = sizeof(WNDCLASSEXW);
        windowClass.lpfnWndProc = WindowProc;
        windowClass.hInstance = GetModuleHandleW(nullptr);
        windowClass.lpszClassName = L"InterviewCheaterClass";

        RegisterClassExW(&windowClass);

        hwnd_ = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOPMOST, L"InterviewCheaterClass", L"Interview Cheater",
                               WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr,
                               nullptr, GetModuleHandleW(nullptr), nullptr);

        if (hwnd_ != nullptr) {
            // Store the this pointer
            SetWindowLongPtr(hwnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

            // Make the window semi-transparent
            SetLayeredWindowAttributes(hwnd_, 0, WINDOW_ALPHA, LWA_ALPHA);
            SetWindowDisplayAffinity(hwnd_, WDA_EXCLUDEFROMCAPTURE);

            // Hide initially
            ShowWindow(hwnd_, SW_HIDE);
            running_ = true;
        }
    }

    void show() {
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

    void update() {  // NOLINT(readability-convert-member-functions-to-static)
        MSG msg;
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    void close() {
        if (hwnd_ != nullptr) {
            DestroyWindow(hwnd_);
            hwnd_ = nullptr;
        }
        running_ = false;
    }

    [[nodiscard]] auto getNativeHandle() const -> void* { return hwnd_; }

    [[nodiscard]] auto isRunning() const -> bool { return running_; }

    void setRunning(bool state) { running_ = state; }

private:
    HWND hwnd_{nullptr};
    bool running_{false};
};

OverlayWindow::OverlayWindow() : pImpl_(std::make_unique<Impl>()) {}

OverlayWindow::~OverlayWindow() = default;

void OverlayWindow::create() { pImpl_->create(); }
void OverlayWindow::show() { pImpl_->show(); }
void OverlayWindow::update() { pImpl_->update(); }
void OverlayWindow::close() { pImpl_->close(); }
auto OverlayWindow::getNativeHandle() const -> void* { return pImpl_->getNativeHandle(); }
auto OverlayWindow::isRunning() const -> bool { return pImpl_->isRunning(); }
void OverlayWindow::setRunning(bool runningState) { pImpl_->setRunning(runningState); }

}  // namespace interview_cheater::window