#pragma once

#include "signal/isignal_manager.hpp"
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
class PALANTIR_CORE_API KeyboardSignalManager : public ISignalManager {
public:
    KeyboardSignalManager();

    /**
     * @brief Destroy the KeyboardSignalManager object.
     *
     * Cleans up all managed signals and releases any platform-specific
     * resources used for signal processing.
     */
    virtual ~KeyboardSignalManager();

    // Delete copy and move operations
    KeyboardSignalManager(const KeyboardSignalManager&) = delete;
    auto operator=(const KeyboardSignalManager&) -> KeyboardSignalManager& = delete;
    KeyboardSignalManager(KeyboardSignalManager&&) = delete;
    auto operator=(KeyboardSignalManager&&) -> KeyboardSignalManager& = delete;

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

private:
    // Forward declaration of platform-specific implementation
    class KeyboardSignalManagerImpl;
    // Suppress C4251 warning for this specific line as Impl clas is never accessed by client
#pragma warning(push)
#pragma warning(disable : 4251)
    std::unique_ptr<KeyboardSignalManagerImpl> pImpl_;  ///< Platform-specific implementation details
#pragma warning(pop)
};

}  // namespace palantir::signal
