#ifndef IINPUT_HPP
#define IINPUT_HPP

class IInput {
public:
    virtual ~IInput() = default;
    
    // Platform-agnostic input state methods
    virtual bool isKeyPressed(int keyCode) const = 0;
    virtual bool isKeyReleased(int keyCode) const = 0;
    virtual bool isModifierActive(int modifier) const = 0;
    
    // Update input state
    virtual void update() = 0;
};

#endif // IINPUT_HPP