#ifndef TOGGLE_SIGNAL_HPP
#define TOGGLE_SIGNAL_HPP

#include "signal/isignal.hpp"
#include "window/window_manager.hpp"
#include "input/iinput.hpp"

class ToggleSignal : public ISignal {
public:
    ToggleSignal(WindowManager& manager, IInput& input);
    ~ToggleSignal() override = default;
    
    void start() override;
    void stop() override;
    bool isActive() const override;
    void check() override;
    
private:
    WindowManager& manager_;
    IInput& input_;
    bool active_{false};
    long long lastTriggerTime_{0};
};

#endif // TOGGLE_SIGNAL_HPP 