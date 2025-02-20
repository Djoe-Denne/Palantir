#ifndef TOGGLE_SIGNAL_HPP
#define TOGGLE_SIGNAL_HPP

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

    void start() override;
    void stop() override;
    bool isActive() const override;
    void check() override;

private:
    window::WindowManager& manager_;
    input::IInput& input_;
    bool active_{false};
    long long lastTriggerTime_{0};
};
}  // namespace signal
}  // namespace interview_cheater
#endif  // TOGGLE_SIGNAL_HPP