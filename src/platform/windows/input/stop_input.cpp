#include "input/stop_input.hpp"

#include <windows.h>

#include <cstdint>

#include "input/key_codes.hpp"

namespace interview_cheater::input {

class StopInput::Impl {
public:
    [[nodiscard]] static auto isKeyPressed() -> bool {
        const auto keyState = static_cast<uint16_t>(GetAsyncKeyState(KeyCodes::KEY_SLASH));
        return (keyState & KeyCodes::KEY_PRESSED_MASK) != 0U;
    }

    [[nodiscard]] static auto isModifierActive() -> bool {
        const auto leftWinPressed = (static_cast<uint16_t>(GetAsyncKeyState(VK_LWIN)) & KeyCodes::KEY_PRESSED_MASK) != 0U;
        const auto rightWinPressed = (static_cast<uint16_t>(GetAsyncKeyState(VK_RWIN)) & KeyCodes::KEY_PRESSED_MASK) != 0U;
        return leftWinPressed || rightWinPressed;
    }

    void update() {}  // No-op for Windows
};

StopInput::StopInput() : pImpl_(std::make_unique<Impl>()) {}
StopInput::~StopInput() = default;

auto StopInput::isKeyPressed() const -> bool { return Impl::isKeyPressed(); }
auto StopInput::isModifierActive() const -> bool { return Impl::isModifierActive(); }
void StopInput::update() { pImpl_->update(); }

}  // namespace interview_cheater::input