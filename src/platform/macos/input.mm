#include "platform/macos/input.hpp"
#include "platform/macos/key_codes.hpp"
#import <Carbon/Carbon.h>

class Input::Impl {
public:
    bool isKeyPressed() const {
        KeyMap keyMap;
        GetKeys(keyMap);
        const uint8_t keyIndex = KeyCodes::KEY_SEMICOLON >> 3;
        const uint8_t keyBit = KeyCodes::KEY_SEMICOLON & 0x7;
        return (((uint8_t*)keyMap)[keyIndex] & (1 << keyBit)) != 0;
    }
    
    bool isModifierActive() const {
        return (CGEventSourceFlagsState(kCGEventSourceStateHIDSystemState) & KeyCodes::COMMAND_MODIFIER) != 0;
    }
    
    void update() {} // No-op for macOS
};

Input::Input() : pImpl(std::make_unique<Impl>()) {}
Input::~Input() = default;

bool Input::isKeyPressed() const { return pImpl->isKeyPressed(); }
bool Input::isModifierActive() const { return pImpl->isModifierActive(); }
void Input::update() { pImpl->update(); } 