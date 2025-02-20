#ifndef IINPUT_HPP
#define IINPUT_HPP

namespace interview_cheater::input {
class IInput {
public:
    virtual ~IInput() = default;

    // Delete copy operations
    IInput(const IInput&) = delete;
    auto operator=(const IInput&) -> IInput& = delete;

    // Define move operations
    IInput(IInput&&) noexcept = default;
    auto operator=(IInput&&) noexcept -> IInput& = default;

    // Platform-agnostic input state methods
    [[nodiscard]] virtual auto isKeyPressed() const -> bool = 0;
    [[nodiscard]] virtual auto isModifierActive() const -> bool = 0;
    virtual auto update() -> void = 0;

protected:
    IInput() = default;
};
}  // namespace interview_cheater::input
#endif  // IINPUT_HPP