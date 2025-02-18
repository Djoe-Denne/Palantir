#include "window/overlay_window.hpp"
#include <windows.h>
#include <dwmapi.h>

namespace {
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        OverlayWindow* window = reinterpret_cast<OverlayWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        
        switch (uMsg) {
            case WM_CLOSE:
                if (window) {
                    window->setRunning(false);
                }
                return 0;
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        }
        
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

class OverlayWindow::Impl {
public:
    Impl() : hwnd(nullptr), running(false) {}
    
    void create() {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = L"InterviewCheaterClass";
        
        RegisterClassEx(&wc);
        
        hwnd = CreateWindowEx(
            WS_EX_LAYERED | WS_EX_TOPMOST,
            L"InterviewCheaterClass",
            L"Interview Cheater",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            800, 600,
            nullptr,
            nullptr,
            GetModuleHandle(nullptr),
            nullptr
        );
        
        if (hwnd) {
            // Store the this pointer
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
            
            // Make the window semi-transparent
            SetLayeredWindowAttributes(hwnd, 0, 240, LWA_ALPHA);
            
            // Exclude from screen capture
            DWORD value = 1;
            DwmSetWindowAttribute(hwnd, DWMWA_CLOAK, &value, sizeof(value));
            
            // Hide initially
            ShowWindow(hwnd, SW_HIDE);
            running = true;
        }
    }
    
    void show() {
        if (!hwnd) return;
        
        if (IsWindowVisible(hwnd)) {
            ShowWindow(hwnd, SW_HIDE);
        } else {
            ShowWindow(hwnd, SW_SHOW);
            SetForegroundWindow(hwnd);
        }
    }
    
    void update() {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    void close() {
        if (hwnd) {
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
        running = state;
    }
    
private:
    HWND hwnd;
    bool running;
};

OverlayWindow::OverlayWindow() : pImpl(std::make_unique<Impl>()) {}
OverlayWindow::~OverlayWindow() = default;

void OverlayWindow::create() { pImpl->create(); }
void OverlayWindow::show() { pImpl->show(); }
void OverlayWindow::update() { pImpl->update(); }
void OverlayWindow::close() { pImpl->close(); }
void* OverlayWindow::getNativeHandle() const { return pImpl->getNativeHandle(); }
bool OverlayWindow::isRunning() const { return pImpl->isRunning(); }
void OverlayWindow::setRunning(bool runningState) { pImpl->setRunning(runningState); } 