#include "input/input.hpp"

#include <windows.h>
#include <cstdint>

#include "input/key_codes.hpp"

namespace interview_cheater::input {

namespace {
constexpr uint16_t KEY_PRESSED_MASK = 0x8000U;
}

class Input::Impl {
public:
    [[nodiscard]] static auto isKeyPressed() -> bool {
        const auto keyState = static_cast<uint16_t>(GetAsyncKeyState(KeyCodes::KEY_F1));
        return (keyState & KEY_PRESSED_MASK) != 0U;
    }

    [[nodiscard]] static auto isModifierActive() -> bool {
        const auto modState = static_cast<uint16_t>(GetAsyncKeyState(KeyCodes::CONTROL_MODIFIER));
        return (modState & KEY_PRESSED_MASK) != 0U;
    }

    void update() {}  // No-op for Windows
};

Input::Input() : pImpl_(std::make_unique<Impl>()) {}
Input::~Input() = default;

auto Input::isKeyPressed() const -> bool { return Impl::isKeyPressed(); }
auto Input::isModifierActive() const -> bool { return Impl::isModifierActive(); }
void Input::update() { pImpl_->update(); }

}  // namespace interview_cheater::input