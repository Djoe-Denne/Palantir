#ifndef WINDOWS_APPLICATION_HPP
#define WINDOWS_APPLICATION_HPP

#include <memory>

#include "platform/application.hpp"

namespace interview_cheater {
namespace signal {
// Forward declaration for SignalManager
class SignalManager;
}  // namespace signal

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
}  // namespace interview_cheater

#endif  // WINDOWS_APPLICATION_HPP