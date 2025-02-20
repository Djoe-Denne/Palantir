#ifndef TOGGLE_SIGNAL_HPP
#define TOGGLE_SIGNAL_HPP

#include <cstdint>

#include "signal/isignal.hpp"

namespace interview_cheater {
namespace window {
// Forward declaration for WindowManager
class WindowManager;
}  // namespace window

namespace input {
// Forward declaration for IInput
class IInput;
}  // namespace input

namespace signal {

class ToggleSignal : public ISignal {
public:
    ToggleSignal(window::WindowManager& manager, input::IInput& input);
    ~ToggleSignal() override = default;

    // Delete copy operations
    ToggleSignal(const ToggleSignal&) = delete;
    auto operator=(const ToggleSignal&) -> ToggleSignal& = delete;

    // Define move operations
    ToggleSignal(ToggleSignal&&) noexcept = default;
    auto operator=(ToggleSignal&&) noexcept -> ToggleSignal& = default;

    auto start() -> void override;
    auto stop() -> void override;
    [[nodiscard]] auto isActive() const -> bool override;
    auto check() -> void override;

private:
    window::WindowManager& manager_;
    input::IInput& input_;
    bool active_{false};
    int64_t lastTriggerTime_{0};
};
}  // namespace signal
}  // namespace interview_cheater
#endif  // TOGGLE_SIGNAL_HPP