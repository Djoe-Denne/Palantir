#ifndef WINDOWS_INPUT_HPP
#define WINDOWS_INPUT_HPP

#include "input/iinput.hpp"

class Input : public IInput {
public:
    bool isCtrlF1Pressed() const override;
};

#endif // WINDOWS_INPUT_HPP