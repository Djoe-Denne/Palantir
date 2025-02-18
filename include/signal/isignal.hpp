#ifndef ISIGNAL_HPP
#define ISIGNAL_HPP

class ISignal {
public:
    virtual ~ISignal() = default;
    
    // Signal lifecycle methods
    virtual void start() = 0;
    virtual void stop() = 0;
    
    // Signal state
    virtual bool isActive() const = 0;
    virtual void check() = 0;
};

#endif // ISIGNAL_HPP