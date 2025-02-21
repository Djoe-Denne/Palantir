#include "signal/stop_signal.hpp"

#include <memory>

#include "application.hpp"
#include "command/stop_command.hpp"
#include "input/iinput.hpp"
#include "utils/logger.hpp"

namespace interview_cheater::signal {

StopSignal::StopSignal(Application& app, input::IInput& input)
    : command_(std::make_unique<command::StopCommand>(app))
    , input_(input) {}

auto StopSignal::start() -> void {
    DEBUG_LOG("Starting StopSignal");
    active_ = true;
}

auto StopSignal::stop() -> void {
    DEBUG_LOG("Stopping StopSignal");
    active_ = false;
}

auto StopSignal::isActive() const -> bool { return active_; }

auto StopSignal::check() -> void {
    if (!active_) {
        return;
    }

    if (input_.isKeyPressed() && input_.isModifierActive()) {
        DEBUG_LOG("Stop signal triggered");
        command_->execute();
    }
}

}  // namespace interview_cheater::signal