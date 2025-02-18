#ifndef CTRL_F1_SIGNAL_HPP
#define CTRL_F1_SIGNAL_HPP

#include "signal/isignal.hpp"
#include "window/window_manager.hpp"
#include "input/iinput.hpp"

class CtrlF1Signal : public ISignal {
public:
    CtrlF1Signal(WindowManager& manager, IInput& input);
    
    void start() override;
    void stop() override;
    bool isActive() const override;
    void check() override;
    
private:
    WindowManager& manager_;
    IInput& input_;
    bool active_;
    long long lastTriggerTime_;  // For debouncing
};

#endif // CTRL_F1_SIGNAL_HPP