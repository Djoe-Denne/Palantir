#include "input/configurable_input.hpp"

#include <windows.h>

#include "input/key_register.hpp"
#include "utils/logger.hpp"

namespace palantir::input {

/**
 * @brief Implementation details for the configurable input.
 *
 * This class handles the internal implementation of configurable input functionality
 * using the PIMPL idiom. It manages the key and modifier codes, and provides methods
 * to check their states using platform-specific APIs.
 */
class ConfigurableInput::Impl {
public:
    // Delete copy operations
    Impl(const Impl& other) = delete;
    auto operator=(const Impl& other) -> Impl& = delete;
    // Delete move operations
    Impl(Impl&& other) noexcept = delete;
    auto operator=(Impl&& other) noexcept -> Impl& = delete;

    /**
     * @brief Construct the implementation object.
     * @param keyCode Virtual key code for the input key.
     * @param modifierCode Virtual key code for the modifier key.
     *
     * Initializes the implementation with the specified key and modifier codes.
     * These codes are used to check the state of keyboard inputs.
     */
    Impl(int keyCode, int modifierCode) : keyCode_(keyCode), modifierCode_(modifierCode) {
        DEBUG_LOG("Initializing configurable input: key=0x{:x}, modifier=0x{:x}", keyCode, modifierCode);
    }

    /**
     * @brief Check if the configured key is currently pressed.
     * @return true if the key is pressed, false otherwise.
     *
     * Uses the Windows GetAsyncKeyState API to check if the configured
     * key is currently in a pressed state.
     */
    [[nodiscard]] auto isKeyPressed(const std::any& event) const -> bool {
        bool pressed = (static_cast<uint16_t>(GetAsyncKeyState(keyCode_)) &
                        static_cast<uint16_t>(KeyRegister::getInstance()->get("KEY_PRESSED_MASK"))) != 0;
        if (pressed) {
            DEBUG_LOG("Key 0x{:x} is pressed", keyCode_);
        }
        return pressed;
    }

    /**
     * @brief Check if the configured modifier is currently active.
     * @return true if the modifier is active, false otherwise.
     *
     * Uses the Windows GetAsyncKeyState API to check if the configured
     * modifier key is currently in a pressed state.
     */
    [[nodiscard]] auto isModifierActive(const std::any& event) const -> bool {
        bool active = (static_cast<uint16_t>(GetAsyncKeyState(modifierCode_)) &
                       static_cast<uint16_t>(KeyRegister::getInstance()->get("KEY_PRESSED_MASK"))) != 0;
        if (active) {
            DEBUG_LOG("Modifier 0x{:x} is active", modifierCode_);
        }
        return active;
    }

    /**
     * @brief Update the input state.
     *
     * Currently a no-op as the Windows GetAsyncKeyState API provides
     * real-time state information without requiring updates.
     */
    auto update() -> void {
        // No update needed for Windows implementation
        // GetAsyncKeyState provides real-time state
    }

    /**
     * @brief Clean up implementation resources.
     *
     * Currently a no-op as the implementation doesn't own any resources.
     */
    ~Impl() { DEBUG_LOG("Destroying configurable input"); }

private:
    int keyCode_;       ///< Virtual key code for the input key
    int modifierCode_;  ///< Virtual key code for the modifier key
};

ConfigurableInput::~ConfigurableInput() = default;

// Public interface implementation
ConfigurableInput::ConfigurableInput(int keyCode, int modifierCode)
    : pImpl_(std::make_unique<Impl>(keyCode, modifierCode)) {
    DEBUG_LOG("Creating configurable input");
}

auto ConfigurableInput::isKeyPressed(const std::any& event) const -> bool {
    return pImpl_->isKeyPressed(event);
}

auto ConfigurableInput::isModifierActive(const std::any& event) const -> bool {
    return pImpl_->isModifierActive(event);
}

auto ConfigurableInput::update() -> void {
    pImpl_->update();
}

}  // namespace palantir::input