#ifndef MACOS_INPUT_HPP
#define MACOS_INPUT_HPP

#include <memory>
#include "input/iinput.hpp"

class Input : public IInput {
public:
    Input();
    ~Input() override;
    
    bool isKeyPressed(int keyCode) const override;
    bool isKeyReleased(int keyCode) const override;
    bool isModifierActive(int modifier) const override;
    void update() override;
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

#endif // MACOS_INPUT_HPP 