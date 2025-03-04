#include "input/configurable_input.hpp"
#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>
#include <any>
#include <memory>
#include "input/key_codes.hpp"
#include "utils/logger.hpp"

namespace palantir::input {

class ConfigurableInput::Impl {
   public:
    explicit Impl(const int keyCode, const int modifierCode) : keyCode_(keyCode), modifierCode_(modifierCode) {
        DEBUG_LOG("Initializing configurable input: key=0x{:x}, modifier=0x{:x}", keyCode, modifierCode);
    }

    Impl(const Impl&) = delete;
    auto operator=(const Impl&) -> Impl& = delete;
    Impl(Impl&&) = delete;
    auto operator=(Impl&&) -> Impl& = delete;

    [[nodiscard]] auto isKeyPressed(const std::any& event) const -> bool {
        try {
            const auto* const nsEvent = std::any_cast<NSEvent*>(event);
            if (nsEvent == nullptr) {
                DEBUG_LOG("Null event received");
                return false;
            }

            if (nsEvent.type == NSEventTypeKeyDown || nsEvent.type == NSEventTypeKeyUp) {
                const bool pressed = (nsEvent.keyCode == keyCode_);
                if (pressed) {
                    DEBUG_LOG("Key 0x{:x} is {}", keyCode_,
                              (nsEvent.type == NSEventTypeKeyDown) ? "pressed" : "released");
                }
                return pressed && (nsEvent.type == NSEventTypeKeyDown);
            }
        } catch (const std::bad_any_cast& e) {
            DEBUG_LOG("Invalid event type in isKeyPressed: {}", e.what());
        }
        return false;
    }

    [[nodiscard]] auto isModifierActive(const std::any& event) const -> bool {
        try {
            const auto* const nsEvent = std::any_cast<NSEvent*>(event);
            if (nsEvent == nullptr) {
                DEBUG_LOG("Null event received");
                return false;
            }

            const bool active = (nsEvent.modifierFlags & static_cast<NSUInteger>(modifierCode_)) != 0;
            if (active) {
                DEBUG_LOG("Modifier 0x{:x} is active", modifierCode_);
            }
            return active;
        } catch (const std::bad_any_cast& e) {
            DEBUG_LOG("Invalid event type in isModifierActive: {}", e.what());
        }
        return false;
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

auto ConfigurableInput::isKeyPressed(const std::any& event) const -> bool { return pImpl_->isKeyPressed(event); }
auto ConfigurableInput::isModifierActive(const std::any& event) const -> bool {
    return pImpl_->isModifierActive(event);
}
auto ConfigurableInput::update() -> void { pImpl_->update(); }

}  // namespace palantir::input