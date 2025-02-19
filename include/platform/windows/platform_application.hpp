#ifndef WINDOWS_APPLICATION_HPP
#define WINDOWS_APPLICATION_HPP

#include "platform/application.hpp"
#include "signal/signal_manager.hpp"
#include <memory>

namespace interview_cheater {
    class PlatformApplication : public Application {
    public:
        explicit PlatformApplication(signal::SignalManager& signalManager);
        ~PlatformApplication() override;
        
        int run() override;
        void quit() override;
        
    private:
        class Impl;
        std::unique_ptr<Impl> pImpl;
    };
}

#endif // WINDOWS_APPLICATION_HPP 