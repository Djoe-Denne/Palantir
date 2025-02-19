#ifndef TOGGLE_SIGNAL_HPP
#define TOGGLE_SIGNAL_HPP

#include "signal/isignal.hpp"
#include "window/window_manager.hpp"
#include "input/iinput.hpp"

namespace interview_cheater::signal {

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

} // namespace interview_cheater::signal

#endif // TOGGLE_SIGNAL_HPP