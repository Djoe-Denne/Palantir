#ifndef MACOS_INPUT_HPP
#define MACOS_INPUT_HPP

#include <memory>

#include "input/iinput.hpp"

namespace interview_cheater::input {
class Input : public IInput {
public:
    Input();
    ~Input() override;

    // Delete copy operations
    Input(const Input&) = delete;
    auto operator=(const Input&) -> Input& = delete;

    // Define move operations
    Input(Input&&) noexcept = default;
    auto operator=(Input&&) noexcept -> Input& = default;

    [[nodiscard]] auto isKeyPressed() const -> bool override;
    [[nodiscard]] auto isModifierActive() const -> bool override;
    auto update() -> void override;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl_;
};
}  // namespace interview_cheater::input
#endif  // MACOS_INPUT_HPP