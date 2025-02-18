#include "signal/toggle_signal.hpp"
#include "command/show_command.hpp"
#include <chrono>

ToggleSignal::ToggleSignal(WindowManager& manager, IInput& input)
    : manager_(manager), input_(input) {}

void ToggleSignal::start() {
    active_ = true;
}

void ToggleSignal::stop() {
    active_ = false;
}

bool ToggleSignal::isActive() const {
    return active_;
}

void ToggleSignal::check() {
    if (!active_) return;
    
    auto currentTime = std::chrono::steady_clock::now().time_since_epoch().count();
    constexpr auto DEBOUNCE_TIME = 300000000LL; // 300ms in nanoseconds
    
    if (input_.isKeyPressed() && input_.isModifierActive()) {
        if (currentTime - lastTriggerTime_ > DEBOUNCE_TIME) {
            if (auto window = manager_.getFirstWindow()) {
                manager_.executeCommand(std::make_unique<ShowCommand>(*window));
                lastTriggerTime_ = currentTime;
            }
        }
    }
} 