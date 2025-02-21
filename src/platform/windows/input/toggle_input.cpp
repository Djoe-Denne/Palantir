#include "input/toggle_input.hpp"

#include <windows.h>

#include <cstdint>

#include "input/key_codes.hpp"

namespace interview_cheater::input {

class ToggleInput::Impl {
public:
    [[nodiscard]] static auto isKeyPressed() -> bool {
        const auto keyState = static_cast<uint16_t>(GetAsyncKeyState(KeyCodes::KEY_F1));
        return (keyState & KeyCodes::KEY_PRESSED_MASK) != 0U;
    }

    [[nodiscard]] static auto isModifierActive() -> bool {
        const auto modState = static_cast<uint16_t>(GetAsyncKeyState(KeyCodes::CONTROL_MODIFIER));
        return (modState & KeyCodes::KEY_PRESSED_MASK) != 0U;
    }

    void update() {}  // No-op for Windows
};

ToggleInput::ToggleInput() : pImpl_(std::make_unique<Impl>()) {}
ToggleInput::~ToggleInput() = default;

auto ToggleInput::isKeyPressed() const -> bool { return Impl::isKeyPressed(); }
auto ToggleInput::isModifierActive() const -> bool { return Impl::isModifierActive(); }
void ToggleInput::update() { pImpl_->update(); }

}  // namespace interview_cheater::input