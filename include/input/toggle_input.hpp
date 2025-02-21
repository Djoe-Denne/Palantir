#ifndef MACOS_TOGGLE_INPUT_HPP
#define MACOS_TOGGLE_INPUT_HPP

#include <memory>

#include "input/iinput.hpp"

namespace interview_cheater::input {
class ToggleInput : public IInput {
public:
    ToggleInput();
    ~ToggleInput() override;

    // Delete copy operations
    ToggleInput(const ToggleInput&) = delete;
    auto operator=(const ToggleInput&) -> ToggleInput& = delete;

    // Define move operations
    ToggleInput(ToggleInput&&) noexcept = default;
    auto operator=(ToggleInput&&) noexcept -> ToggleInput& = default;

    [[nodiscard]] auto isKeyPressed() const -> bool override;
    [[nodiscard]] auto isModifierActive() const -> bool override;
    auto update() -> void override;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl_;
};
}  // namespace interview_cheater::input
#endif  // MACOS_TOGGLE_INPUT_HPP