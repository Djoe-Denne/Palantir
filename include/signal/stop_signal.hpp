#ifndef STOP_SIGNAL_HPP
#define STOP_SIGNAL_HPP

#include <cstdint>
#include <memory>

#include "signal/isignal.hpp"

namespace interview_cheater {
class Application;  // Forward declaration

namespace input {
class IInput;  // Forward declaration
}  // namespace input

namespace command {
class StopCommand;  // Forward declaration
}  // namespace command

namespace signal {

class StopSignal : public ISignal {
public:
    StopSignal(Application& app, input::IInput& input);
    ~StopSignal() override = default;

    // Delete copy operations
    StopSignal(const StopSignal&) = delete;
    auto operator=(const StopSignal&) -> StopSignal& = delete;

    // Define move operations
    StopSignal(StopSignal&&) noexcept = default;
    auto operator=(StopSignal&&) noexcept -> StopSignal& = default;

    auto start() -> void override;
    auto stop() -> void override;
    [[nodiscard]] auto isActive() const -> bool override;
    auto check() -> void override;

private:
    std::unique_ptr<command::StopCommand> command_;
    input::IInput& input_;
    bool active_{false};
    int64_t lastTriggerTime_{0};
};

}  // namespace signal
}  // namespace interview_cheater

#endif  // STOP_SIGNAL_HPP