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
#include <core_export.hpp>
#include <memory>

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
    /**
     * @brief Get the singleton instance of SignalManager
     * @return Reference to the singleton instance
     */
    static auto getInstance() -> std::shared_ptr<SignalManager>;

    /**
     * @brief Set the singleton instance of SignalManager
     * @param instance The instance to set
     */
    static auto setInstance(const std::shared_ptr<SignalManager>& instance) -> void;

    /**
     * @brief Destroy the SignalManager object.
     *
     * Cleans up all managed signals and releases any platform-specific
     * resources used for signal processing.
     */
    virtual ~SignalManager();

    // Delete copy and move operations
    SignalManager(const SignalManager&) = delete;
    auto operator=(const SignalManager&) -> SignalManager& = delete;
    SignalManager(SignalManager&&) = delete;
    auto operator=(SignalManager&&) = delete;

    /**
     * @brief Add a new signal to be managed.
     * @param signal Unique pointer to the signal to be added.
     */
    virtual auto addSignal(std::unique_ptr<ISignal> signal) -> void;

    /**
     * @brief Start processing all managed signals.
     */
    virtual auto startSignals() const -> void;

    /**
     * @brief Stop processing all managed signals.
     */
    virtual auto stopSignals() const -> void;

    /**
     * @brief Check all managed signals.
     */
    virtual auto checkSignals(const std::any& event) const -> void;

protected:
    // Private constructor for singleton
    SignalManager();

private:
    // Forward declaration of platform-specific implementation
    class Impl;
    // Suppress C4251 warning for this specific line as Impl clas is never accessed by client
#pragma warning(push)
#pragma warning(disable : 4251)
    std::unique_ptr<Impl> pImpl_;  ///< Platform-specific implementation details
    static std::shared_ptr<SignalManager> instance_;
#pragma warning(pop)
};

}  // namespace palantir::signal

#endif  // SIGNAL_MANAGER_HPP