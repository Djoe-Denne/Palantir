#include "windows/input/input.hpp"
#include <windows.h>

bool Input::isCtrlF1Pressed() const {
    return (GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_F1) & 0x8000);
}