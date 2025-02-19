#include "window/overlay_window.hpp"
#include <dwmapi.h>
#include <windows.h>
#import "utils/logger.hpp"


namespace interview_cheater::window
{
namespace
{
auto CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    auto* window =
        reinterpret_cast<OverlayWindow*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));  // NOLINT(performance-no-int-to-ptr)

    switch (uMsg)
    {
        case WM_CLOSE:
            if (window != nullptr)
            {
                window->setRunning(false);
            }
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            if (window != nullptr)
            {
                PAINTSTRUCT paintStruct;
                HDC         hdc = BeginPaint(hwnd, &paintStruct);
                RECT        rect;
                GetClientRect(hwnd, &rect);
                int    width  = rect.right;
                int    height = rect.bottom;
                HBRUSH brush  = CreateSolidBrush(RGB(255, 0, 0));
                RECT   square = {width / 4, height / 4, 3 * width / 4, 3 * height / 4};
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
}  // namespace

class OverlayWindow::Impl
{
public:
    Impl() = default;

    void create()
    {
        WNDCLASSEXW windowClass   = {};
        windowClass.cbSize        = sizeof(WNDCLASSEXW);
        windowClass.lpfnWndProc   = WindowProc;
        windowClass.hInstance     = GetModuleHandleW(nullptr);
        windowClass.lpszClassName = L"InterviewCheaterClass";

        RegisterClassExW(&windowClass);

        // todo: move these to a separate file
        constexpr int windowWidth  = 800;
        constexpr int windowHeight = 600;

        hwnd = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOPMOST,
                               L"InterviewCheaterClass",
                               L"Interview Cheater",
                               WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT,
                               CW_USEDEFAULT,
                               windowWidth,
                               windowHeight,
                               nullptr,
                               nullptr,
                               GetModuleHandleW(nullptr),
                               nullptr);

        if (hwnd != nullptr)
        {
            // Store the this pointer
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

            // Make the window semi-transparent
            constexpr BYTE kAlphaValue = 240;
            SetLayeredWindowAttributes(hwnd, 0, kAlphaValue, LWA_ALPHA);
            SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);

            // Hide initially
            ShowWindow(hwnd, SW_HIDE);
            running = true;
        }
    }

    void show()
    {
        if (hwnd == nullptr)
        {
            return;
        }

        if (IsWindowVisible(hwnd) == TRUE)
        {
            ShowWindow(hwnd, SW_HIDE);
        }
        else
        {
            ShowWindow(hwnd, SW_SHOW);
            SetForegroundWindow(hwnd);
            UpdateWindow(hwnd);  // Ensure the window is updated
        }
    }

    void update()
    {  // NOLINT(readability-convert-member-functions-to-static)
        MSG msg;
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    void close()
    {
        if (hwnd != nullptr)
        {
            DestroyWindow(hwnd);
            hwnd = nullptr;
        }
        running = false;
    }

    [[nodiscard]] auto getNativeHandle() const -> void*
    {
        return hwnd;
    }

    [[nodiscard]] auto isRunning() const -> bool
    {
        return running;
    }

    void setRunning(bool state)
    {
        running = state;
    }

private:
    HWND hwnd    = nullptr;
    bool running = false;
};

OverlayWindow::OverlayWindow() : pImpl(std::make_unique<Impl>()), running(false) {}
OverlayWindow::~OverlayWindow() = default;

void OverlayWindow::create()
{
    pImpl->create();
}
void OverlayWindow::show()
{
    pImpl->show();
}
void OverlayWindow::update()
{
    pImpl->update();
}
void OverlayWindow::close()
{
    pImpl->close();
}
auto OverlayWindow::getNativeHandle() const -> void*
{
    return pImpl->getNativeHandle();
}
auto OverlayWindow::isRunning() const -> bool
{
    return pImpl->isRunning();
}
void OverlayWindow::setRunning(bool runningState)
{
    pImpl->setRunning(runningState);
}

}  // namespace interview_cheater::window