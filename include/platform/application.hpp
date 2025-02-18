#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>
#include "signal/signal_manager.hpp"

class Application {
public:
    virtual ~Application() = default;
    
    // Get the singleton instance
    static std::shared_ptr<Application> getInstance(SignalManager& signalManager);
    
    // Platform-agnostic application lifecycle methods
    virtual int run() = 0;
    virtual void quit() = 0;
    
protected:
    Application() = default;
    
private:
    static std::shared_ptr<Application> instance;
};

#endif // APPLICATION_HPP 