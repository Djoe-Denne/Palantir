#include "signal/toggle_signal.hpp"

#include <chrono>
#include <memory>

#include "command/show_command.hpp"
#include "input/iinput.hpp"
#include "window/window_manager.hpp"
#include "utils/logger.hpp"

namespace interview_cheater::signal {

ToggleSignal::ToggleSignal(window::WindowManager& manager, input::IInput& input)
    : command_(std::make_unique<command::ShowCommand>(manager))
    , input_(input) {}

auto ToggleSignal::start() -> void {
    DEBUG_LOG("Starting ToggleSignal");
    active_ = true;
}

auto ToggleSignal::stop() -> void {
    DEBUG_LOG("Stopping ToggleSignal");
    active_ = false;
}

auto ToggleSignal::isActive() const -> bool { return active_; }

auto ToggleSignal::check() -> void {
    if (!active_) {
        return;
    }

    auto currentTime = std::chrono::steady_clock::now().time_since_epoch().count();
    constexpr auto DEBOUNCE_TIME = 300000000LL;  // 300ms in nanoseconds

    if (input_.isKeyPressed() && input_.isModifierActive()) {
        if (currentTime - lastTriggerTime_ > DEBOUNCE_TIME) {
            DEBUG_LOG("Toggle signal triggered");
            command_->execute();
            lastTriggerTime_ = currentTime;
        }
    }
}

}  // namespace interview_cheater::signal