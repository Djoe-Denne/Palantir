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

    // Delete copy operations
    PlatformApplication(const PlatformApplication&) = delete;
    auto operator=(const PlatformApplication&) -> PlatformApplication& = delete;

    // Define move operations
    PlatformApplication(PlatformApplication&&) noexcept = default;
    auto operator=(PlatformApplication&&) noexcept -> PlatformApplication& = default;

    auto run() -> int override;
    auto quit() -> void override;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl_;
};
}  // namespace interview_cheater

#endif  // WINDOWS_APPLICATION_HPP