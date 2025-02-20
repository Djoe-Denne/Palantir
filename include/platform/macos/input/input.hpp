#ifndef MACOS_INPUT_HPP
#define MACOS_INPUT_HPP

#include <memory>

#include "input/iinput.hpp"

namespace interview_cheater::input {
class Input : public IInput {
public:
    Input();
    ~Input() override;

    bool isKeyPressed() const override;
    bool isModifierActive() const override;
    void update() override;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};
}  // namespace interview_cheater::input
#endif  // MACOS_INPUT_HPP