#ifndef WINDOWS_APPLICATION_HPP
#define WINDOWS_APPLICATION_HPP

#include "platform/application.hpp"
#include <memory>

namespace interview_cheater {
    namespace signal {
    // Forward declaration for SignalManager
    class SignalManager;
    }

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