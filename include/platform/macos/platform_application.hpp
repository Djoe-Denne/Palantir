#ifndef MACOS_APPLICATION_HPP
#define MACOS_APPLICATION_HPP

#include "platform/application.hpp"
#include "signal/signal_manager.hpp"

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

#endif // MACOS_APPLICATION_HPP 