
#include "platform_application.hpp"
#include <windows.h>

namespace interview_cheater {

class PlatformApplication::Impl {
public:
    Impl(signal::SignalManager& signalManager) : signalManager_(signalManager), running_(true) {}
    
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
    signal::SignalManager& signalManager_;
    bool running_;
};

PlatformApplication::PlatformApplication(signal::SignalManager& signalManager)
    : pImpl(std::make_unique<Impl>(signalManager)) {}

PlatformApplication::~PlatformApplication() = default;

int PlatformApplication::run() {
    return pImpl->run();
}

void PlatformApplication::quit() {
    pImpl->quit();
}

} // namespace interview_cheater