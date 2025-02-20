#ifndef MACOS_APPLICATION_HPP
#define MACOS_APPLICATION_HPP

#include "platform/application.hpp"
#include "signal/signal_manager.hpp"

namespace interview_cheater {
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

#endif  // MACOS_APPLICATION_HPP