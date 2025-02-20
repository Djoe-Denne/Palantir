#include "platform_application.hpp"
#include "utils/logger.hpp"
#include "signal/signal_manager.hpp"
#include <Windows.h>

namespace {

constexpr int K_SLASH_KEY_CODE = 0xBF;  // VK_OEM_2 - '/' key
constexpr int K_DEBOUNCE_TIME = 100;   // milliseconds

} // namespace

namespace interview_cheater {

class PlatformApplication::Impl {
public:
    explicit Impl(signal::SignalManager& signalManager) 
        : signalManager_(signalManager)
        , lastTriggerTime_(0) {}

    auto run() -> int {
        DEBUG_LOG("Starting Windows message loop");
        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            // Check for Command (Windows key) + / combination
            if ((GetAsyncKeyState(VK_LWIN) & 0x8000) || (GetAsyncKeyState(VK_RWIN) & 0x8000)) {
                if (GetAsyncKeyState(K_SLASH_KEY_CODE) & 0x8000) {
                    auto currentTime = GetTickCount();
                    if (currentTime - lastTriggerTime_ > K_DEBOUNCE_TIME) {
                        DEBUG_LOG("Hotkey combination detected (Win + /)");
                        signalManager_.checkSignals();
                        lastTriggerTime_ = currentTime;
                    }
                }
            }
        }
        return 0;
    }

private:
    signal::SignalManager& signalManager_;
    DWORD lastTriggerTime_;
};

PlatformApplication::PlatformApplication(signal::SignalManager& signalManager)
    : pImpl_(std::make_unique<Impl>(signalManager)) {}

PlatformApplication::~PlatformApplication() = default;

auto PlatformApplication::run() -> int {
    return pImpl_->run();
}

auto PlatformApplication::quit() -> void {
    PostQuitMessage(0);
}

}  // namespace interview_cheater
