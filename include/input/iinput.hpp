#ifndef IINPUT_HPP
#define IINPUT_HPP

class IInput {
public:
    virtual ~IInput() = default;
    virtual bool isCtrlF1Pressed() const = 0;
};

#endif // IINPUT_HPP