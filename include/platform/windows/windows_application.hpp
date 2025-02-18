#ifndef WINDOWS_APPLICATION_HPP
#define WINDOWS_APPLICATION_HPP

#include "platform/application.hpp"
#include "signal/signal_manager.hpp"
#include <memory>

class WindowsApplication : public Application {
public:
    explicit WindowsApplication(SignalManager& signalManager);
    ~WindowsApplication() override;
    
    int run() override;
    void quit() override;
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

#endif // WINDOWS_APPLICATION_HPP 