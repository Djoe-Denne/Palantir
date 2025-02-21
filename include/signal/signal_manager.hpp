/**
 * @file signal_manager.hpp
 * @brief Defines the signal management system.
 *
 * This file contains the SignalManager class which is responsible for managing
 * the lifecycle and processing of all signals in the application. It handles
 * signal registration, activation, and checking.
 */

#ifndef SIGNAL_MANAGER_HPP
#define SIGNAL_MANAGER_HPP

#include <memory>
#include <vector>

namespace interview_cheater::signal {
// Forward declaration for ISignal
class ISignal;

/**
 * @class SignalManager
 * @brief Manager class for handling multiple signals.
 *
 * This class manages a collection of signals, controlling their lifecycle
 * and processing. It provides methods to add new signals, start and stop
 * signal processing, and check signal conditions. The class supports move
 * semantics but prohibits copying to ensure unique ownership of signals.
 */
class SignalManager {
public:
    /**
     * @brief Construct a new SignalManager object.
     *
     * Initializes the signal manager and sets up any necessary platform-specific
     * resources for signal processing.
     */
    SignalManager();

    /**
     * @brief Destroy the SignalManager object.
     *
     * Cleans up all managed signals and releases any platform-specific
     * resources used for signal processing.
     */
    ~SignalManager();

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent resource duplication. */
    SignalManager(const SignalManager&) = delete;
    /** @brief Deleted copy assignment to prevent resource duplication. */
    auto operator=(const SignalManager&) -> SignalManager& = delete;

    // Define move operations
    /** @brief Default move constructor for transfer of manager ownership. */
    SignalManager(SignalManager&&) noexcept = default;
    /** @brief Default move assignment for transfer of manager ownership. */
    auto operator=(SignalManager&&) noexcept -> SignalManager& = default;

    /**
     * @brief Add a new signal to the manager.
     * @param signal Unique pointer to the signal to add.
     *
     * Takes ownership of the provided signal and adds it to the collection
     * of managed signals. The signal will be included in future start, stop,
     * and check operations.
     */
    auto addSignal(std::unique_ptr<ISignal> signal) -> void;

    /**
     * @brief Start all managed signals.
     *
     * Activates all signals in the manager, allowing them to begin monitoring
     * for their trigger conditions. This is typically called when the
     * application starts or resumes processing.
     */
    auto startSignals() -> void;

    /**
     * @brief Stop all managed signals.
     *
     * Deactivates all signals in the manager, causing them to cease monitoring
     * for their trigger conditions. This is typically called when the
     * application is shutting down or pausing processing.
     */
    auto stopSignals() -> void;

    /**
     * @brief Check all managed signals.
     *
     * Triggers a check of all active signals to see if their conditions are
     * met. This is typically called in response to input events or during
     * the application's update cycle.
     */
    auto checkSignals() -> void;

private:
    /** @brief Collection of managed signals. */
    std::vector<std::unique_ptr<ISignal>> signals_;
};

}  // namespace interview_cheater::signal

#endif  // SIGNAL_MANAGER_HPP