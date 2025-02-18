#ifndef WINDOWS_INPUT_HPP
#define WINDOWS_INPUT_HPP

#include <memory>
#include "input/iinput.hpp"

class Input : public IInput {
public:
    Input();
    ~Input() override = default;
    
    bool isKeyPressed() const override;
    bool isModifierActive() const override;
    void update() override;
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

#endif // WINDOWS_INPUT_HPP 