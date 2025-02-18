#include "platform/windows/windows_application.hpp"
#include "utils/logger.hpp"
#include <windows.h>

class WindowsApplication::Impl {
public:
    Impl(SignalManager& signalManager) : signalManager_(signalManager), running_(true) {
        DEBUG_LOG("Initializing WindowsApplication");
    }
    
    int run() {
        DEBUG_LOG("Starting application run loop");
        while (running_) {
            MSG msg;
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    DEBUG_LOG("Received quit message");
                    running_ = false;
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            
            // Check signals every frame
            signalManager_.checkSignals();
            
            // Small sleep to prevent high CPU usage
            Sleep(10);
        }
        DEBUG_LOG("Application run loop ended");
        return 0;
    }
    
    void quit() {
        DEBUG_LOG("Application quitting");
        running_ = false;
        PostQuitMessage(0);
    }
    
    ~Impl() {
        DEBUG_LOG("Destroying WindowsApplication");
    }
    
private:
    SignalManager& signalManager_;
    bool running_;
};

WindowsApplication::WindowsApplication(SignalManager& signalManager)
    : pImpl(std::make_unique<Impl>(signalManager)) {
    DEBUG_LOG("Creating WindowsApplication");
}

WindowsApplication::~WindowsApplication() = default;

int WindowsApplication::run() {
    return pImpl->run();
}

void WindowsApplication::quit() {
    pImpl->quit();
} 