#include "signal/toggle_signal.hpp"

#include <chrono>

#include "command/show_command.hpp"
#include "input/iinput.hpp"
#include "window/window_manager.hpp"

namespace interview_cheater {
signal::ToggleSignal::ToggleSignal(window::WindowManager& manager, input::IInput& input)
    : manager_(manager), input_(input) {}

void signal::ToggleSignal::start() { active_ = true; }

void signal::ToggleSignal::stop() { active_ = false; }

auto signal::ToggleSignal::isActive() const -> bool { return active_; }

void signal::ToggleSignal::check() {
    if (!active_) {
        return;
    }

    auto currentTime = std::chrono::steady_clock::now().time_since_epoch().count();
    constexpr auto DEBOUNCE_TIME = 300000000LL;  // 300ms in nanoseconds

    if (input_.isKeyPressed() && input_.isModifierActive()) {
        if (currentTime - lastTriggerTime_ > DEBOUNCE_TIME) {
            if (auto* window = manager_.getFirstWindow()) {
                manager_.executeCommand(std::make_unique<command::ShowCommand>(*window));
                lastTriggerTime_ = currentTime;
            }
        }
    }
}
}  // namespace interview_cheater