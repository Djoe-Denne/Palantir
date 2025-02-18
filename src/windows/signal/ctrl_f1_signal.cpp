#include "signal/ctrl_f1_signal.hpp"
#include "input/iinput.hpp"
#include "command/stop_command.hpp"
#include <windows.h>

HHOOK CtrlF1Signal::hHook_ = nullptr;
WindowManager* CtrlF1Signal::manager_ = nullptr;
IInput* CtrlF1Signal::input_ = nullptr;

CtrlF1Signal::CtrlF1Signal(WindowManager& manager, IInput& input) {
    manager_ = &manager;
    input_ = &input;
}

CtrlF1Signal::~CtrlF1Signal() {
    if (hHook_) {
        UnhookWindowsHookEx(hHook_);
    }
}

void CtrlF1Signal::start() {
    hHook_ = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, 0);
}

LRESULT CALLBACK CtrlF1Signal::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        if (wParam == WM_KEYDOWN && pKeyboard->vkCode == VK_F1 && (GetAsyncKeyState(VK_CONTROL) & 0x8000)) {
            if (manager_ && input_) {
                auto window = manager_->getFirstWindow();
                if (window) {
                    window->setRunning(false);
                    PostQuitMessage(0);
                }
            }
        }
    }
    return CallNextHookEx(hHook_, nCode, wParam, lParam);
}