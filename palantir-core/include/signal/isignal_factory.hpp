/**
 * @file isignal_factory.hpp
 * @brief Defines the factory for creating signal handlers.
 *
 * This file contains the ISignalFactory class which is responsible for creating
 * signal handlers that connect inputs to commands. It manages the creation of
 * different types of signals based on the application's needs.
 */
#pragma once

#include <memory>
#include <vector>

#include "core_export.hpp"
#include "signal/isignal.hpp"

namespace palantir::signal {

/**
 * @class SignalFactory
 * @brief Factory class for creating signal handlers.
 *
 * This factory class is responsible for creating and managing signal
 * handlers that connect inputs to commands. It provides methods to create
 * specific types of signals as well as collections of signals based on
 * configuration.
 */
class PALANTIR_CORE_API ISignalFactory {
public:
    /** @brief Virtual destructor. */
    virtual ~ISignalFactory() = default;

    // Delete copy operations
    /** @brief Deleted copy constructor. */
    ISignalFactory(const ISignalFactory&) = delete;
    /** @brief Deleted copy assignment. */
    auto operator=(const ISignalFactory&) -> ISignalFactory& = delete;

    // Delete move operations
    /** @brief Deleted move constructor. */
    ISignalFactory(ISignalFactory&&) = delete;
    /** @brief Deleted move assignment. */
    auto operator=(ISignalFactory&&) -> ISignalFactory& = delete;

    /**
     * @brief Create all configured signals for the application.
     * @param app Shared pointer to the application instance.
     * @return Vector of unique pointers to created signals.
     *
     * Creates and returns all signals configured for the application.
     * This includes both toggle and stop signals, configured according
     * to the application's settings.
     */
    [[nodiscard]] virtual auto createSignals() const -> std::vector<std::unique_ptr<ISignal>> = 0;

protected:
    /** @brief Protected default constructor to prevent direct instantiation. */
    ISignalFactory() = default;
};

}  // namespace palantir::signal
