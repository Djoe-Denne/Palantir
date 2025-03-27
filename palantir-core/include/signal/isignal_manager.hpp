#pragma once

#include <any>
#include <core_export.hpp>
#include <memory>

namespace palantir::signal {
// Forward declaration for ISignal
class ISignal;

/**
 * @class ISignalManager
 * @brief Interface for signal management.
 *
 * This class defines the interface for signal management. It provides methods
 * for adding signals, starting and stopping signal processing, and checking
*/
class PALANTIR_CORE_API ISignalManager {
public:
    /**
     * @brief Destroy the ISignalManager object.
     *
     * Cleans up all managed signals and releases any platform-specific
     * resources used for signal processing.
     */
    virtual ~ISignalManager() = default;

    // Delete copy and move operations
    ISignalManager(const ISignalManager&) = delete;
    auto operator=(const ISignalManager&) -> ISignalManager& = delete;
    ISignalManager(ISignalManager&&) = delete;
    auto operator=(ISignalManager&&) -> ISignalManager& = delete;

    /**
     * @brief Add a new signal to be managed.
     * @param signal Unique pointer to the signal to be added.
     */
    virtual auto addSignal(std::unique_ptr<ISignal> signal) -> void = 0;

    /**
     * @brief Start processing all managed signals.
     */
    virtual auto startSignals() const -> void = 0;

    /**
     * @brief Stop processing all managed signals.
     */
    virtual auto stopSignals() const -> void = 0;

    /**
     * @brief Check all managed signals.
     */
    virtual auto checkSignals(const std::any& event) const -> void = 0;

protected:
    ISignalManager() = default;
};

}  // namespace palantir::signal
