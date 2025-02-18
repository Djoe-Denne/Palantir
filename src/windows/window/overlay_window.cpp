#include "windows/window/overlay_window.hpp"
#include <iostream>
#include <dwmapi.h>
#include "include/cef_app.h"
#include "include/wrapper/cef_helpers.h"
#include "include/cef_client.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"

#define WINDOW_SIZE_PERCENT 50

class SimpleHandler : public CefClient, public CefLifeSpanHandler {
public:
    SimpleHandler() = default;
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
        return this;
    }
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override {
        CEF_REQUIRE_UI_THREAD();
        browser_list_.push_back(browser);
    }
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override {
        CEF_REQUIRE_UI_THREAD();
        auto it = std::find(browser_list_.begin(), browser_list_.end(), browser);
        if (it != browser_list_.end()) {
            browser_list_.erase(it);
        }
        if (browser_list_.empty()) {
            CefQuitMessageLoop();
        }
    }
private:
    std::vector<CefRefPtr<CefBrowser>> browser_list_;
    IMPLEMENT_REFCOUNTING(SimpleHandler);
};

OverlayWindow::OverlayWindow() : hwnd_(nullptr), running_(true) {}

void OverlayWindow::create(HINSTANCE hInstance) {
    const wchar_t CLASS_NAME[] = L"OverlayWindowClass";
    WNDCLASS wc = {};
    wc.lpfnWndProc = windowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    
    if (!RegisterClass(&wc)) {
        std::cerr << "Failed to register window class." << std::endl;
        return;
    }
    
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int overlaySize = screenWidth * WINDOW_SIZE_PERCENT / 100;
    
    hwnd_ = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        CLASS_NAME, L"Overlay",
        WS_POPUP,
        (screenWidth - overlaySize) / 2, (screenHeight - overlaySize) / 2,
        overlaySize, overlaySize,
        NULL, NULL, hInstance, NULL
    );
    
    if (!hwnd_) {
        std::cerr << "Failed to create window." << std::endl;
        return;
    }
    
    if (!SetLayeredWindowAttributes(hwnd_, 0, 180, LWA_ALPHA)) {
        std::cerr << "Failed to set layered window attributes." << std::endl;
        return;
    }
    
    if (!SetWindowDisplayAffinity(hwnd_, WDA_EXCLUDEFROMCAPTURE)) {
        std::cerr << "Failed to set window display affinity." << std::endl;
        return;
    }

    CefMainArgs main_args(hInstance);
    CefRefPtr<CefApp> app;
    CefSettings settings;
    settings.no_sandbox = true;
    
    if (!CefInitialize(main_args, settings, app, nullptr)) {
        std::cerr << "Failed to initialize CEF." << std::endl;
        return;
    }

    CefWindowInfo window_info;
    window_info.SetAsChild(hwnd_, {0, 0, overlaySize, overlaySize});
    CefBrowserSettings browser_settings;
    CefRefPtr<SimpleHandler> handler(new SimpleHandler());
    
    if (!CefBrowserHost::CreateBrowser(window_info, handler, "http://www.google.fr", browser_settings, nullptr, nullptr)) {
        std::cerr << "Failed to create CEF browser." << std::endl;
        return;
    }
}

void OverlayWindow::show() {
    ShowWindow(hwnd_, SW_SHOW);
    std::cout << "Overlay window shown." << std::endl;
    update();
}

void OverlayWindow::update() {
    UpdateWindow(hwnd_);
}

void OverlayWindow::close() {
    running_ = false;
    DestroyWindow(hwnd_);
    CefShutdown();
}

HWND OverlayWindow::getHandle() const {
    return hwnd_;
}

bool OverlayWindow::isRunning() const {
    return running_;
}

void OverlayWindow::setRunning(bool runningState) {
    running_ = runningState;
}

// Define the windowProc function
LRESULT CALLBACK OverlayWindow::windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
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
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}