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

#include <any>
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

    // Default move operations
    /** @brief Default move constructor for ownership transfer. */
    SignalManager(SignalManager&&) noexcept = default;
    /** @brief Default move assignment for ownership transfer. */
    auto operator=(SignalManager&&) noexcept -> SignalManager& = default;

    /**
     * @brief Add a new signal to be managed.
     * @param signal Unique pointer to the signal to be added.
     *
     * Takes ownership of the provided signal and adds it to the collection
     * of managed signals.
     */
    auto addSignal(std::unique_ptr<ISignal> signal) -> void;

    /**
     * @brief Start processing all managed signals.
     *
     * Activates all managed signals, allowing them to begin processing
     * their respective conditions.
     */
    auto startSignals() -> void;

    /**
     * @brief Stop processing all managed signals.
     *
     * Deactivates all managed signals, stopping them from processing
     * their respective conditions.
     */
    auto stopSignals() -> void;

    /**
     * @brief Check all managed signals.
     *
     * Triggers a check of conditions for all managed signals.
     */
    auto checkSignals(const std::any& event) -> void;

private:
    // Forward declaration of platform-specific implementation
    class Impl;
    std::unique_ptr<Impl> pImpl_;                    ///< Platform-specific implementation details
    std::vector<std::unique_ptr<ISignal>> signals_;  ///< Collection of managed signals
};

}  // namespace interview_cheater::signal

#endif  // SIGNAL_MANAGER_HPP