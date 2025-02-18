#include "platform/windows/windows_application.hpp"
#include <windows.h>

class WindowsApplication::Impl {
public:
    Impl(SignalManager& signalManager) : signalManager_(signalManager), running_(true) {}
    
    int run() {
        while (running_) {
            MSG msg;
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
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
        return 0;
    }
    
    void quit() {
        running_ = false;
        PostQuitMessage(0);
    }
    
private:
    SignalManager& signalManager_;
    bool running_;
};

WindowsApplication::WindowsApplication(SignalManager& signalManager)
    : pImpl(std::make_unique<Impl>(signalManager)) {}

WindowsApplication::~WindowsApplication() = default;

int WindowsApplication::run() {
    return pImpl->run();
}

void WindowsApplication::quit() {
    pImpl->quit();
} 