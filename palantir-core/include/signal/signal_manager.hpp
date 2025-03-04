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
#include <core_export.hpp>

namespace palantir::signal {
// Forward declaration for ISignal
class ISignal;

/**
 * @class SignalManager
 * @brief Manager class for handling multiple signals.
 *
 * This class manages a collection of signals, controlling their lifecycle
 * and processing. It provides methods to add new signals, start and stop
 * signal processing, and check signal conditions. The class is implemented
 * as a singleton with PIMPL idiom to prevent C4251 warnings.
 */
class PALANTIR_CORE_API SignalManager {
public:
    // Forward declaration of platform-specific implementation
    class Impl;
    /**
     * @brief Get the singleton instance of SignalManager
     * @return Reference to the singleton instance
     */
    static auto getInstance() -> SignalManager&;

    /**
     * @brief Destroy the SignalManager object.
     *
     * Cleans up all managed signals and releases any platform-specific
     * resources used for signal processing.
     */
    ~SignalManager();

    // Delete copy and move operations
    SignalManager(const SignalManager&) = delete;
    auto operator=(const SignalManager&) -> SignalManager& = delete;
    SignalManager(SignalManager&&) = delete;
    auto operator=(SignalManager&&) = delete;

    /**
     * @brief Add a new signal to be managed.
     * @param signal Unique pointer to the signal to be added.
     */
    auto addSignal(std::unique_ptr<ISignal> signal) -> void;

    /**
     * @brief Start processing all managed signals.
     */
    auto startSignals() -> void;

    /**
     * @brief Stop processing all managed signals.
     */
    auto stopSignals() -> void;

    /**
     * @brief Check all managed signals.
     */
    auto checkSignals(const std::any& event) -> void;

private:
    // Private constructor for singleton
    SignalManager();

    // Suppress C4251 warning for this specific line as Impl clas is never accessed by client
#pragma warning(push)
#pragma warning(disable: 4251)
    std::unique_ptr<Impl> pImpl_;  ///< Platform-specific implementation details
#pragma warning(pop)
};

}  // namespace palantir::signal

#endif  // SIGNAL_MANAGER_HPP