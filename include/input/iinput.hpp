#ifndef IINPUT_HPP
#define IINPUT_HPP

namespace interview_cheater::input {
class IInput {
public:
    virtual ~IInput() = default;

    // Platform-agnostic input state methods
    virtual bool isKeyPressed() const = 0;
    virtual bool isModifierActive() const = 0;
    virtual void update() = 0;
};
}  // namespace interview_cheater::input
#endif  // IINPUT_HPP