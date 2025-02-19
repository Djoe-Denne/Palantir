#include "input/input.hpp"

#include "input/key_codes.hpp"

#include <windows.h>

namespace interview_cheater::input
{
class Input::Impl
{
public:
    [[nodiscard]] static auto isKeyPressed() -> bool
    {
        constexpr uint16_t KEY_PRESSED_MASK = 0x8000;
        return (static_cast<uint16_t>(GetAsyncKeyState(KeyCodes::KEY_F1)) & KEY_PRESSED_MASK) != 0;
    }

    [[nodiscard]] static auto isModifierActive() -> bool
    {
        constexpr uint16_t KEY_PRESSED_MASK = 0x8000;
        return (static_cast<uint16_t>(GetAsyncKeyState(KeyCodes::CONTROL_MODIFIER)) & KEY_PRESSED_MASK) != 0;
    }

    void update() {}  // No-op for Windows
};

Input::Input() : pImpl(std::make_unique<Impl>()) {}
Input::~Input() = default;

auto Input::isKeyPressed() const -> bool
{
    return Impl::isKeyPressed();
}
auto Input::isModifierActive() const -> bool
{
    return Impl::isModifierActive();
}
void Input::update()
{
    pImpl->update();
}

}  // namespace interview_cheater::input