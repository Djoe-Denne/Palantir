#ifndef SIGNAL_MANAGER_HPP
#define SIGNAL_MANAGER_HPP

#include <memory>
#include <vector>

namespace interview_cheater::signal {
// Forward declaration for ISignal
class ISignal;

class SignalManager {
public:
    SignalManager() = default;
    ~SignalManager() = default;

    // Delete copy operations
    SignalManager(const SignalManager&) = delete;
    auto operator=(const SignalManager&) -> SignalManager& = delete;

    // Define move operations
    SignalManager(SignalManager&&) noexcept = default;
    auto operator=(SignalManager&&) noexcept -> SignalManager& = default;

    auto addSignal(std::unique_ptr<ISignal> signal) -> void;
    auto startSignals() -> void;
    auto stopSignals() -> void;
    auto checkSignals() -> void;

private:
    std::vector<std::unique_ptr<ISignal>> signals_;
};

}  // namespace interview_cheater::signal

#endif  // SIGNAL_MANAGER_HPP