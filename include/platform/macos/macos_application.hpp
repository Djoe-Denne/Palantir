#ifndef MACOS_APPLICATION_HPP
#define MACOS_APPLICATION_HPP

#include "platform/application.hpp"
#include "signal/signal_manager.hpp"

class MacOSApplication : public Application {
public:
    explicit MacOSApplication(SignalManager& signalManager);
    ~MacOSApplication() override;
    
    int run() override;
    void quit() override;
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

#endif // MACOS_APPLICATION_HPP 