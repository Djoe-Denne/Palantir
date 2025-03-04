#include "signal/signal.hpp"

#include <chrono>

#include "command/icommand.hpp"
#include "input/iinput.hpp"
#include "utils/logger.hpp"

namespace palantir::signal {

Signal::Signal(std::unique_ptr<input::IInput> input, std::unique_ptr<command::ICommand> command, const bool useDebounce)
    : input_(std::move(input)), command_(std::move(command)), useDebounce_(useDebounce) {
    DEBUG_LOG("Creating signal");
}

auto Signal::start() -> void {
    DEBUG_LOG("Starting signal");
    active_ = true;
}

auto Signal::stop() -> void {
    DEBUG_LOG("Stopping signal");
    active_ = false;
}

[[nodiscard]] auto Signal::isActive() const -> bool { return active_; }

auto Signal::check(const std::any& event) -> void {
    if (!active_ || !input_ || !command_) {
        return;
    }

    if (input_->isModifierActive(event) && input_->isKeyPressed(event)) {
        const auto currentTime = std::chrono::steady_clock::now().time_since_epoch().count();

        if (!useDebounce_ || (currentTime - lastTriggerTime_ > DEBOUNCE_TIME)) {
            DEBUG_LOG("Signal triggered");
            command_->execute();
            lastTriggerTime_ = currentTime;
        }
    }
}

}  // namespace palantir::signal