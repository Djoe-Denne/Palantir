#include "platform/macos/input.hpp"
#include "platform/macos/key_codes.hpp"
#import <Carbon/Carbon.h>

class Input::Impl {
public:
    bool isKeyPressed(int keyCode) const {
        KeyMap keyMap;
        GetKeys(keyMap);
        uint8_t keyIndex = keyCode >> 3;  // Divide by 8
        uint8_t keyBit = keyCode & 0x7;   // Modulo 8
        return (((uint8_t*)keyMap)[keyIndex] & (1 << keyBit)) != 0;
    }
    
    bool isKeyReleased(int keyCode) const {
        return !isKeyPressed(keyCode);
    }
    
    bool isModifierActive(int modifier) const {
        return (CGEventSourceFlagsState(kCGEventSourceStateHIDSystemState) & modifier) != 0;
    }
    
    void update() {
        // Nothing to do for macOS implementation
    }
};

Input::Input() : pImpl(std::make_unique<Impl>()) {}
Input::~Input() = default;

bool Input::isKeyPressed(int keyCode) const {
    return pImpl->isKeyPressed(keyCode);
}

bool Input::isKeyReleased(int keyCode) const {
    return pImpl->isKeyReleased(keyCode);
}

bool Input::isModifierActive(int modifier) const {
    return pImpl->isModifierActive(modifier);
}

void Input::update() {
    pImpl->update();
} 