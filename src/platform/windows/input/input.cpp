#include "input/input.hpp"
#include "input/key_codes.hpp"
#include <windows.h>

namespace interview_cheater::input {

class Input::Impl {
public:
    bool isKeyPressed() const {
        return (GetAsyncKeyState(KeyCodes::KEY_F1) & 0x8000) != 0;
    }
    
    bool isModifierActive() const {
        return (GetAsyncKeyState(KeyCodes::CONTROL_MODIFIER) & 0x8000) != 0;
    }
    
    void update() {} // No-op for Windows
};

Input::Input() : pImpl(std::make_unique<Impl>()) {}
Input::~Input() = default;

bool Input::isKeyPressed() const { return pImpl->isKeyPressed(); }
bool Input::isModifierActive() const { return pImpl->isModifierActive(); }
void Input::update() { pImpl->update(); }

} // namespace interview_cheater::input