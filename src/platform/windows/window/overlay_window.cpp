#include "window/overlay_window.hpp"
#include "utils/logger.hpp"
#include <windows.h>
#include <dwmapi.h>

namespace {
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        OverlayWindow* window = reinterpret_cast<OverlayWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        
        switch (uMsg) {
            case WM_CLOSE:
                if (window) {
                    DEBUG_LOG("Window close requested");
                    window->setRunning(false);
                }
                return 0;
            case WM_DESTROY:
                DEBUG_LOG("Window being destroyed");
                PostQuitMessage(0);
                return 0;
            case WM_PAINT:
                if (window) {
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);
                    RECT rect;
                    GetClientRect(hwnd, &rect);
                    int width = rect.right;
                    int height = rect.bottom;
                    HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
                    RECT square = {width / 4, height / 4, 3 * width / 4, 3 * height / 4};
                    FillRect(hdc, &square, brush);
                    DeleteObject(brush);
                    EndPaint(hwnd, &ps);
                    return 0;
                }
                return 0;
        }
        
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
}

class OverlayWindow::Impl {
public:
    Impl() : hwnd(nullptr), running(false) {
        DEBUG_LOG("Creating OverlayWindow implementation");
    }
    
    void create() {
        DEBUG_LOG("Beginning window creation");
        WNDCLASSEXW wc = {};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandleW(nullptr);
        wc.lpszClassName = L"InterviewCheaterClass";
        
        RegisterClassExW(&wc);
        
        hwnd = CreateWindowExW(
            WS_EX_LAYERED | WS_EX_TOPMOST,
            L"InterviewCheaterClass",
            L"Interview Cheater",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            800, 600,
            nullptr,
            nullptr,
            GetModuleHandleW(nullptr),
            nullptr
        );
        
        if (hwnd) {
            DEBUG_LOG("Window created successfully");
            // Store the this pointer
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
            
            // Make the window semi-transparent
            SetLayeredWindowAttributes(hwnd, 0, 240, LWA_ALPHA);
            SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);
            
            // Hide initially
            ShowWindow(hwnd, SW_HIDE);
            running = true;
            DEBUG_LOG("Window initialization complete");
        } else {
            DEBUG_LOG("Failed to create window");
        }
    }
    
    void show() {
        if (!hwnd) return;
        
        if (IsWindowVisible(hwnd)) {
            DEBUG_LOG("Hiding window");
            ShowWindow(hwnd, SW_HIDE);
        } else {
            DEBUG_LOG("Showing window");
            ShowWindow(hwnd, SW_SHOW);
            SetForegroundWindow(hwnd);
            UpdateWindow(hwnd);
        }
    }
    
    void update() {
        MSG msg;
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    
    void close() {
        if (hwnd) {
            DEBUG_LOG("Closing window");
            DestroyWindow(hwnd);
            hwnd = nullptr;
        }
        running = false;
    }
    
    void* getNativeHandle() const {
        return hwnd;
    }
    
    bool isRunning() const {
        return running;
    }
    
    void setRunning(bool state) {
        DEBUG_LOG("Setting running state to: %d", state);
        running = state;
    }
    
    ~Impl() {
        DEBUG_LOG("Destroying window implementation");
        if (hwnd) {
            DestroyWindow(hwnd);
            hwnd = nullptr;
        }
    }
    
private:
    HWND hwnd;
    bool running;
};

OverlayWindow::OverlayWindow() : pImpl(std::make_unique<Impl>()) {
    DEBUG_LOG("Creating OverlayWindow");
}
OverlayWindow::~OverlayWindow() = default;

void OverlayWindow::create() { pImpl->create(); }
void OverlayWindow::show() { pImpl->show(); }
void OverlayWindow::update() { pImpl->update(); }
void OverlayWindow::close() { pImpl->close(); }
void* OverlayWindow::getNativeHandle() const { return pImpl->getNativeHandle(); }
bool OverlayWindow::isRunning() const { return pImpl->isRunning(); }
void OverlayWindow::setRunning(bool runningState) { pImpl->setRunning(runningState); }