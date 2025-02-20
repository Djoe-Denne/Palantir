#include "platform_application.hpp"

#include <Windows.h>

#include <cstdint>

#include "signal/signal_manager.hpp"
#include "utils/logger.hpp"

namespace {

constexpr int K_SLASH_KEY_CODE = 0xBF;  // VK_OEM_2 - '/' key
constexpr int K_DEBOUNCE_TIME = 100;    // milliseconds
constexpr uint16_t KEY_PRESSED_MASK = 0x8000U;

}  // namespace

namespace interview_cheater {

class PlatformApplication::Impl {
public:
    explicit Impl(signal::SignalManager& signalManager) : signalManager_(signalManager) {}

    auto run() -> int {
        DEBUG_LOG("Starting Windows message loop");
        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            // Check for Command (Windows key) + / combination
            const auto leftWinPressed = (static_cast<uint16_t>(GetAsyncKeyState(VK_LWIN)) & KEY_PRESSED_MASK) != 0U;
            const auto rightWinPressed = (static_cast<uint16_t>(GetAsyncKeyState(VK_RWIN)) & KEY_PRESSED_MASK) != 0U;
            const auto slashPressed =
                (static_cast<uint16_t>(GetAsyncKeyState(K_SLASH_KEY_CODE)) & KEY_PRESSED_MASK) != 0U;

            if ((leftWinPressed || rightWinPressed) && slashPressed) {
                const auto currentTime = GetTickCount();
                if (currentTime - lastTriggerTime_ > K_DEBOUNCE_TIME) {
                    DEBUG_LOG("Hotkey combination detected (Win + /)");
                    signalManager_.checkSignals();
                    lastTriggerTime_ = currentTime;
                }
            }
        }
        return 0;
    }

private:
    signal::SignalManager& signalManager_;
    DWORD lastTriggerTime_ = 0;
};

PlatformApplication::PlatformApplication(signal::SignalManager& signalManager)
    : pImpl_(std::make_unique<Impl>(signalManager)) {}

PlatformApplication::~PlatformApplication() = default;

auto PlatformApplication::run() -> int { return pImpl_->run(); }

auto PlatformApplication::quit() -> void { PostQuitMessage(0); }

}  // namespace interview_cheater
