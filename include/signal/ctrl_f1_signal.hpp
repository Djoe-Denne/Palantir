#ifndef CTRL_F1_SIGNAL_HPP
#define CTRL_F1_SIGNAL_HPP

#include "signal/isignal.hpp"
#include "window/window_manager.hpp"
#include "windows/input/input.hpp"
#include <windows.h>

class CtrlF1Signal : public ISignal {
public:
    CtrlF1Signal(WindowManager& manager, Input& input);
    ~CtrlF1Signal();
    void start() override;

private:
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK hHook_;
    static WindowManager* manager_;
    static Input* input_;
};

#endif // CTRL_F1_SIGNAL_HPP