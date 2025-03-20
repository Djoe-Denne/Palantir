#include "input/keyboard_Input.hpp"
#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>
#include <any>
#include <memory>
#include "input/key_codes.hpp"
#include "utils/logger.hpp"

namespace palantir::input {

class KeyboardInput::Impl {
   public:
    explicit Impl(const int keyCode, const int modifierCode) : keyCode_(keyCode), modifierCode_(modifierCode) {
        DebugLog("Initializing configurable input: key=0x{:x}, modifier=0x{:x}", keyCode, modifierCode);
    }

    Impl(const Impl&) = delete;
    auto operator=(const Impl&) -> Impl& = delete;
    Impl(Impl&&) = delete;
    auto operator=(Impl&&) -> Impl& = delete;

    [[nodiscard]] auto isKeyPressed(const std::any& event) const -> bool {
        try {
            const auto* const nsEvent = std::any_cast<NSEvent*>(event);
            if (nsEvent == nullptr) {
                DebugLog("Null event received");
                return false;
            }

            if (nsEvent.type == NSEventTypeKeyDown || nsEvent.type == NSEventTypeKeyUp) {
                const bool pressed = (nsEvent.keyCode == keyCode_);
                if (pressed) {
                    DebugLog("Key 0x{:x} is {}", keyCode_,
                              (nsEvent.type == NSEventTypeKeyDown) ? "pressed" : "released");
                }
                return pressed && (nsEvent.type == NSEventTypeKeyDown);
            }
        } catch (const std::bad_any_cast& e) {
            DebugLog("Invalid event type in isKeyPressed: {}", e.what());
        }
        return false;
    }

    [[nodiscard]] auto isModifierActive(const std::any& event) const -> bool {
        try {
            const auto* const nsEvent = std::any_cast<NSEvent*>(event);
            if (nsEvent == nullptr) {
                DebugLog("Null event received");
                return false;
            }

            const bool active = (nsEvent.modifierFlags & static_cast<NSUInteger>(modifierCode_)) != 0;
            if (active) {
                DebugLog("Modifier 0x{:x} is active", modifierCode_);
            }
            return active;
        } catch (const std::bad_any_cast& e) {
            DebugLog("Invalid event type in isModifierActive: {}", e.what());
        }
        return false;
    }

    auto update() -> void {
        // No update needed for macOS implementation
        // CGEventSource provides real-time state
    }

    ~Impl() { DebugLog("Destroying configurable input"); }

   private:
    int keyCode_;       ///< Virtual key code for the input key
    int modifierCode_;  ///< Virtual key code for the modifier key
};

KeyboardInput::KeyboardInput(int keyCode, int modifierCode)
    : pImpl_(std::make_unique<Impl>(keyCode, modifierCode)) {
    DebugLog("Creating configurable input");
}

KeyboardInput::~KeyboardInput() = default;

auto KeyboardInput::isKeyPressed(const std::any& event) const -> bool { return pImpl_->isKeyPressed(event); }
auto KeyboardInput::isModifierActive(const std::any& event) const -> bool {
    return pImpl_->isModifierActive(event);
}
auto KeyboardInput::update() -> void { pImpl_->update(); }

}  // namespace palantir::input