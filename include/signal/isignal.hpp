#ifndef ISIGNAL_HPP
#define ISIGNAL_HPP

namespace interview_cheater::signal {

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

} // namespace interview_cheater::signal

#endif // ISIGNAL_HPP