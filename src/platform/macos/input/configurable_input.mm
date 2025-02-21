#include "input/configurable_input.hpp"
#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>
#include "input/key_codes.hpp"
#include "utils/logger.hpp"

namespace interview_cheater::input {

class ConfigurableInput::Impl {
   public:
    Impl(int keyCode, int modifierCode) : keyCode_(keyCode), modifierCode_(modifierCode) {
        DEBUG_LOG("Initializing configurable input: key=0x{:x}, modifier=0x{:x}", keyCode, modifierCode);
    }

    [[nodiscard]] auto isKeyPressed() const -> bool {
        CGEventFlags modifiers = CGEventSourceFlagsState(kCGEventSourceStateHIDSystemState);
        bool pressed = (CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, keyCode_) != 0);
        if (pressed) {
            DEBUG_LOG("Key 0x{:x} is pressed", keyCode_);
        }
        return pressed;
    }

    [[nodiscard]] auto isModifierActive() const -> bool {
        CGEventFlags modifiers = CGEventSourceFlagsState(kCGEventSourceStateHIDSystemState);
        bool active = (modifiers & modifierCode_) != 0;
        if (active) {
            DEBUG_LOG("Modifier 0x{:x} is active", modifierCode_);
        }
        return active;
    }

    auto update() -> void {
        // No update needed for macOS implementation
        // CGEventSource provides real-time state
    }

    ~Impl() { DEBUG_LOG("Destroying configurable input"); }

   private:
    int keyCode_;       ///< Virtual key code for the input key
    int modifierCode_;  ///< Virtual key code for the modifier key
};

ConfigurableInput::ConfigurableInput(int keyCode, int modifierCode)
    : pImpl_(std::make_unique<Impl>(keyCode, modifierCode)) {
    DEBUG_LOG("Creating configurable input");
}

ConfigurableInput::~ConfigurableInput() = default;

auto ConfigurableInput::isKeyPressed() const -> bool { return pImpl_->isKeyPressed(); }
auto ConfigurableInput::isModifierActive() const -> bool { return pImpl_->isModifierActive(); }
auto ConfigurableInput::update() -> void { pImpl_->update(); }

}  // namespace interview_cheater::input