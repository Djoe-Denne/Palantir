#ifndef ISIGNAL_HPP
#define ISIGNAL_HPP

class ISignal {
public:
    virtual ~ISignal() = default;
    virtual void start() = 0;
};

#endif // ISIGNAL_HPP