#pragma once

#include <any>
#include <core_export.hpp>
#include <memory>

#include "signal/isignal_manager.hpp"
#include "signal/keyboard_api.hpp"
#include "signal/isignal_factory.hpp"
#include "signal/keyboard_signal_factory.hpp"

namespace palantir::signal {
// Forward declaration for ISignal
class ISignal;

/**
 * @class KeyboardSignalManager
 * @brief Manager class for handling multiple signals that respond to keyboard events.
 *
 * This class manages a collection of signals, controlling their lifecycle
 * and processing. It provides methods to start and stop
 * signal processing, and check signal conditions. The class is implemented
 * using the PIMPL idiom to hide platform-specific implementation details.
 */
class PALANTIR_CORE_API KeyboardSignalManager : public ISignalManager {
public:
    /**
     * @brief Default constructor
     * 
     * Creates a standard keyboard signal manager with a default KeyboardSignalFactory
     */
    KeyboardSignalManager();

    /**
     * @brief Construct a new KeyboardSignalManager object
     * @param keyboardApi Optional custom keyboard API implementation (for testing)
     *
     * Default constructor creates a standard keyboard API wrapper for the current platform.
     * For testing, a mock implementation can be provided.
     */
    explicit KeyboardSignalManager(std::unique_ptr<KeyboardApi> keyboardApi);

    /**
     * @brief Construct a new KeyboardSignalManager with a custom factory
     * @param factory Shared pointer to an ISignalFactory implementation
     */
    explicit KeyboardSignalManager(const std::shared_ptr<ISignalFactory>& factory);

    /**
     * @brief Construct a new KeyboardSignalManager with a custom factory and API
     * @param factory Shared pointer to an ISignalFactory implementation
     * @param keyboardApi Optional custom keyboard API implementation (for testing)
     */
    KeyboardSignalManager(const std::shared_ptr<ISignalFactory>& factory, std::unique_ptr<KeyboardApi> keyboardApi);

    /**
     * @brief Destroy the KeyboardSignalManager object.
     *
     * Cleans up all managed signals and releases any platform-specific
     * resources used for signal processing.
     */
    ~KeyboardSignalManager() override;

    // Delete copy and move operations
    KeyboardSignalManager(const KeyboardSignalManager&) = delete;
    auto operator=(const KeyboardSignalManager&) -> KeyboardSignalManager& = delete;
    KeyboardSignalManager(KeyboardSignalManager&&) = delete;
    auto operator=(KeyboardSignalManager&&) -> KeyboardSignalManager& = delete;

    /**
     * @brief Start processing all managed signals.
     * If no signals are present, will use the factory to create them.
     */
    auto startSignals() const -> void override;

    /**
     * @brief Stop processing all managed signals.
     */
    auto stopSignals() const -> void override;

    /**
     * @brief Check all managed signals.
     */
    auto checkSignals(const std::any& event) const -> void override;

private:
    // Forward declaration of platform-specific implementation
    class KeyboardSignalManagerImpl;
    // Suppress C4251 warning for this specific line as Impl clas is never accessed by client
#pragma warning(push)
#pragma warning(disable : 4251)
    std::unique_ptr<KeyboardSignalManagerImpl> pImpl_;  ///< Platform-specific implementation details
    std::shared_ptr<ISignalFactory> factory_;  ///< Signal factory for creating signals
#pragma warning(pop)
};

}  // namespace palantir::signal
