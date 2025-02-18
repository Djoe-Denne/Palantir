#include "signal/ctrl_f1_signal.hpp"
#include "command/show_command.hpp"

#ifdef _WIN32
#include "platform/windows/key_codes.hpp"
#elif defined(__APPLE__)
#include "platform/macos/key_codes.hpp"
#endif

#include <chrono>

CtrlF1Signal::CtrlF1Signal(WindowManager& manager, IInput& input)
    : manager_(manager), input_(input), active_(false), lastTriggerTime_(0) {}

void CtrlF1Signal::start() {
    active_ = true;
}

void CtrlF1Signal::stop() {
    active_ = false;
}

bool CtrlF1Signal::isActive() const {
    return active_;
}

void CtrlF1Signal::check() {
    if (!active_) return;
    
#ifdef _WIN32
    // Windows: Ctrl+F1
    constexpr int KEY = VK_F1;
    constexpr int MODIFIER = VK_CONTROL;
#elif defined(__APPLE__)
    // macOS: Command+Semicolon
    constexpr int KEY = KeyCodes::KEY_SEMICOLON;
    constexpr int MODIFIER = KeyCodes::COMMAND_MODIFIER;
#endif

    auto currentTime = std::chrono::steady_clock::now().time_since_epoch().count();
    constexpr auto DEBOUNCE_TIME = 300000000LL; // 300ms in nanoseconds
    
    if (input_.isKeyPressed(KEY) && input_.isModifierActive(MODIFIER)) {
        // Check if enough time has passed since last trigger
        if (currentTime - lastTriggerTime_ > DEBOUNCE_TIME) {
            auto window = manager_.getFirstWindow();
            if (window) {
                auto command = std::make_unique<ShowCommand>(*window);
                manager_.executeCommand(std::move(command));
                lastTriggerTime_ = currentTime;
            }
        }
    }
} 