#ifndef STOP_INPUT_HPP
#define STOP_INPUT_HPP

#include <memory>

#include "input/iinput.hpp"

namespace interview_cheater::input {
class StopInput : public IInput {
public:
    StopInput();
    ~StopInput() override;

    // Delete copy operations
    StopInput(const StopInput&) = delete;
    auto operator=(const StopInput&) -> StopInput& = delete;

    // Define move operations
    StopInput(StopInput&&) noexcept = default;
    auto operator=(StopInput&&) noexcept -> StopInput& = default;

    [[nodiscard]] auto isKeyPressed() const -> bool override;
    [[nodiscard]] auto isModifierActive() const -> bool override;
    auto update() -> void override;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl_;
};
}  // namespace interview_cheater::input
#endif  // STOP_INPUT_HPP