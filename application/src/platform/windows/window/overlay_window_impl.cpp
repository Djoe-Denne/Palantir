#include "window/overlay_window_impl.hpp"

#include <dwmapi.h>
#include <windows.h>
#include <windowsx.h>  // For GET_X_LPARAM and GET_Y_LPARAM

#include <algorithm>  // For std::max

#include "utils/logger.hpp"
#include "window/component/content_manager.hpp"
#include "window/component/webview/webview.hpp"

namespace palantir::window {
namespace {

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr BYTE WINDOW_ALPHA = 240;
constexpr int SQUARE_MARGIN_RATIO = 4;  // Divides width/height by this to get margin
constexpr COLORREF SQUARE_COLOR = RGB(255, 0, 0);

}  // namespace

OverlayWindow::Impl::Impl()  // NOLINT
    : contentManager_(new component::ContentManager<component::webview::WebView>()) {}

OverlayWindow::Impl::~Impl() {
    // Make sure we unregister as observer first
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
                // DEBUG_LOG("Resizing WebView2 to %ldx%ld", bounds.right - bounds.left, bounds.bottom - bounds.top);
                // contentManager_->resize(bounds.right - bounds.left, bounds.bottom - bounds.top);
            }
            return 0;
        case WM_ERASEBKGND:
            return 1;  // Prevent background erasing to avoid flicker
        // Add handlers for frameless window dragging
        case WM_NCHITTEST: {
            // Get cursor position
            POINT point;
            point.x = GET_X_LPARAM(lParam);
            point.y = GET_Y_LPARAM(lParam);

            // Get window rect
            RECT rcWindow;
            GetWindowRect(hwnd, &rcWindow);

            // Check if cursor is in the right frame area
            if (point.x >= rcWindow.right - RIGHT_FRAME_WIDTH && point.x <= rcWindow.right) {
                // If in the right frame area, allow dragging
                int windowHeight = rcWindow.bottom - rcWindow.top;
                int frameHeight = std::min(RIGHT_FRAME_WIDTH, windowHeight);

                // If in the vertical center of the right frame (limited to frameHeight)
                int verticalCenter = (rcWindow.top + rcWindow.bottom) / 2;
                if (point.y >= verticalCenter - frameHeight / 2 && point.y <= verticalCenter + frameHeight / 2) {
                    return HTCAPTION;  // Allow dragging from this area
                }

                return HTRIGHT;  // Allow resizing from right edge
            }

            // Let the default procedure handle resizing areas
            LRESULT result = DefWindowProcW(hwnd, uMsg, wParam, lParam);
            if (result == HTCLIENT) {
                // If in client area, treat it as caption for dragging
                return HTCAPTION;
            }
            return result;
        }
        case WM_NCCALCSIZE: {
            // Handling non-client area calculations
            if (wParam == TRUE) {
                auto* params = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);  // NOLINT

                // Save the original right edge position
                LONG originalRight = params->rgrc[0].right;

                // Let Windows handle the calculation first
                LRESULT result = DefWindowProcW(hwnd, uMsg, wParam, lParam);

                // Restore the right edge to keep our frame
                params->rgrc[0].right = originalRight;

                // Adjust the client area to exclude the right frame
                params->rgrc[0].right -= RIGHT_FRAME_WIDTH;

                return result;
            }
            break;
        }
        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

auto OverlayWindow::Impl::create() -> void {
    DEBUG_LOG("Starting overlay window creation");

    WNDCLASSEXW windowClass = {};
    windowClass.cbSize = sizeof(WNDCLASSEXW);
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = GetModuleHandleW(nullptr);
    windowClass.lpszClassName = L"PalantirClass";
    windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);  // NOLINT
    windowClass.style = CS_HREDRAW | CS_VREDRAW;

    if (RegisterClassExW(&windowClass) == 0) {
        DWORD error = GetLastError();
        DEBUG_LOG("Failed to register window class: %lu", error);
        throw std::runtime_error("Failed to register window class");
    }

    // Create window with WS_POPUP style and WS_THICKFRAME to allow resizing
    hwnd_ = CreateWindowExW(WS_EX_LAYERED, L"PalantirClass", L"Palantir",
                            WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_THICKFRAME, CW_USEDEFAULT, CW_USEDEFAULT,
                            WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, GetModuleHandleW(nullptr), this);

    if (hwnd_ == nullptr) {
        DWORD error = GetLastError();
        DEBUG_LOG("Failed to create window: %lu", error);
        throw std::runtime_error("Failed to create window");
    }

    DEBUG_LOG("Window created successfully with handle: %p", hwnd_);

    // Make the window frameless with a semi-transparent background, but keep right frame
    makeWindowFrameless();

    // Set window transparency
    SetLayeredWindowAttributes(hwnd_, 0, WINDOW_ALPHA, LWA_ALPHA);

    // Set up Webview
    contentManager_->initialize(hwnd_);

    // Make sure the window is always on top
    SetWindowPos(hwnd_, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    // Position the window in the center of the screen
    RECT windowRect;
    GetWindowRect(hwnd_, &windowRect);
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    SetWindowPos(hwnd_, nullptr, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    // Update the running state
    running_ = true;
}

auto OverlayWindow::Impl::makeWindowFrameless() -> void {
    if (hwnd_ == nullptr) {
        return;
    }

    // Enable DWM shadows but keep a small frame on the right side
    // Left, Top, Right, Bottom
    MARGINS margins = {0, 0, -RIGHT_FRAME_WIDTH,
                       1};  // Negative right margin to keep frame, slight bottom margin for shadow
    DwmExtendFrameIntoClientArea(hwnd_, &margins);

    // Enable DWM blur behind (Windows 10 only)
    HMODULE hUser32 = GetModuleHandleW(L"user32.dll");
    if (hUser32 != nullptr) {
        typedef BOOL(WINAPI * SetWindowCompositionAttribute)(HWND, void*);
        auto pSetWindowCompositionAttribute =
            reinterpret_cast<SetWindowCompositionAttribute>(GetProcAddress(hUser32, "SetWindowCompositionAttribute"));

        if (pSetWindowCompositionAttribute != nullptr) {
            // Define the necessary structures
            struct WindowCompositionAttributeData {
                DWORD Attrib;
                PVOID pvData;
                SIZE_T cbData;
            };

            struct AccentPolicy {
                DWORD AccentState;
                DWORD AccentFlags;
                DWORD GradientColor;
                DWORD AnimationId;
            };

            // Set the accent policy
            AccentPolicy policy = {};
            policy.AccentState = 3;                    // ACCENT_ENABLE_BLURBEHIND
            policy.GradientColor = TRANSPARENT_COLOR;  // Transparent

            WindowCompositionAttributeData data = {};
            data.Attrib = 19;  // WCA_ACCENT_POLICY // NOLINT
            data.pvData = &policy;
            data.cbData = sizeof(policy);

            pSetWindowCompositionAttribute(hwnd_, &data);
        }
    }
}

auto OverlayWindow::Impl::updateWindowSize(int contentWidth, int contentHeight) -> void {
    if (hwnd_ == nullptr) {
        return;
    }

    // Calculate the new window size (content + padding + right frame)
    int newWidth = contentWidth + (BORDER_PADDING * 2) + RIGHT_FRAME_WIDTH;
    int newHeight = contentHeight + (BORDER_PADDING * 2);

    // Only resize if the size has actually changed
    if (newWidth != currentWidth_ || newHeight != currentHeight_) {
        DEBUG_LOG("Resizing window to %dx%d", newWidth, newHeight);

        // Get current window position to maintain the same center point
        RECT windowRect;
        GetWindowRect(hwnd_, &windowRect);
        int oldWidth = windowRect.right - windowRect.left;
        int oldHeight = windowRect.bottom - windowRect.top;

        // Calculate new position to keep window centered
        int posX = windowRect.left - ((newWidth - oldWidth) / 2);
        int posY = windowRect.top - ((newHeight - oldHeight) / 2);

        // If window is too small, enforce minimum size
        newWidth = std::max(newWidth, 200);
        newHeight = std::max(newHeight, 150);

        // Resize and reposition the window
        SetWindowPos(hwnd_, nullptr, posX, posY, newWidth, newHeight, SWP_NOZORDER);

        // Update current size
        currentWidth_ = newWidth;
        currentHeight_ = newHeight;
    }
}

auto OverlayWindow::Impl::getCurrentScreenResolution() -> std::pair<int, int> {
    if (hwnd_ == nullptr) {
        return {0, 0};  // Return invalid resolution if no window handle exists
    }

    // Get the monitor where the window is currently displayed
    HMONITOR monitor = MonitorFromWindow(hwnd_, MONITOR_DEFAULTTONEAREST);
    if (monitor == nullptr) {
        return {0, 0};  // Return invalid resolution if monitor not found
    }

    // Get monitor info
    MONITORINFO monitorInfo = {sizeof(MONITORINFO)};
    if (GetMonitorInfo(monitor, &monitorInfo)) {
        int width = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
        int height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
        return {width, height};
    }

    return {0, 0};  // Return invalid resolution if something goes wrong
}

auto OverlayWindow::Impl::show() -> void {
    if (hwnd_ != nullptr) {
        if (IsWindowVisible(hwnd_) == TRUE) {
            ShowWindow(hwnd_, SW_HIDE);
        } else {
            ShowWindow(hwnd_, SW_SHOW);
            SetForegroundWindow(hwnd_);
        }
    }
}

auto OverlayWindow::Impl::update() -> void {
    if (hwnd_ != nullptr) {
        return;
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
    if (hwnd_ == nullptr) {
        return;
    }

    uintptr_t style = GetWindowLongPtr(hwnd_, GWL_EXSTYLE);
    if (isToolWindow) {
        SetWindowLongPtr(hwnd_, GWL_EXSTYLE, style | WS_EX_TOOLWINDOW);
    } else {
        SetWindowLongPtr(hwnd_, GWL_EXSTYLE, style & ~WS_EX_TOOLWINDOW);  // NOLINT
    }
}

auto OverlayWindow::Impl::getNativeHandle() const -> void* { return hwnd_; }

auto OverlayWindow::Impl::isRunning() const -> bool { return running_; }

auto OverlayWindow::Impl::setRunning(bool state) -> void { running_ = state; }

auto OverlayWindow::Impl::getContentManager() const -> std::shared_ptr<component::IContentManager> {
    return contentManager_;
}

}  // namespace palantir::window
