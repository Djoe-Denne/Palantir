#ifndef IINPUT_HPP
#define IINPUT_HPP

class IInput {
public:
    virtual ~IInput() = default;
    
    // Platform-agnostic input state methods
    virtual bool isKeyPressed() const = 0;
    virtual bool isModifierActive() const = 0;
    virtual void update() = 0;
};

#endif // IINPUT_HPP