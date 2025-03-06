/**
 * @file signal_factory.hpp
 * @brief Defines the factory for creating signal handlers.
 *
 * This file contains the SignalFactory class which is responsible for creating
 * signal handlers that connect inputs to commands. It manages the creation of
 * different types of signals based on the application's needs.
 */

#ifndef SIGNAL_FACTORY_HPP
#define SIGNAL_FACTORY_HPP

#include <memory>
#include <vector>

#include "signal/isignal.hpp"
#include "application.hpp"
#include "window/window_manager.hpp"
#include "core_export.hpp"

namespace palantir::signal {

/**
 * @class SignalFactory
 * @brief Factory class for creating signal handlers.
 *
 * This static factory class is responsible for creating and managing signal
 * handlers that connect inputs to commands. It provides methods to create
 * specific types of signals as well as collections of signals based on
 * configuration. The class is non-instantiable and provides only static methods.
 */
class PALANTIR_CORE_API SignalFactory {
public:
    virtual ~SignalFactory();

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent instantiation. */
    SignalFactory(const SignalFactory&) = delete;
    /** @brief Deleted copy assignment to prevent instantiation. */
    auto operator=(const SignalFactory&) -> SignalFactory& = delete;

    // Delete move operations
    /** @brief Deleted move constructor to prevent instantiation. */
    SignalFactory(SignalFactory&&) = delete;
    /** @brief Deleted move assignment to prevent instantiation. */
    auto operator=(SignalFactory&&) -> SignalFactory& = delete;

    /** 
     * @brief Get the singleton instance of the SignalFactory.
     * @return A shared pointer to the SignalFactory instance.
     *
     * Returns the singleton instance of the SignalFactory.
     */
    [[nodiscard]] static auto getInstance() -> std::shared_ptr<SignalFactory>;

    /** 
     * @brief Set the singleton instance of the SignalFactory.
     * @param instance A shared pointer to the SignalFactory instance.
     *
     * Sets the singleton instance of the SignalFactory.
     */
    static auto setInstance(const std::shared_ptr<SignalFactory>& instance) -> void;

    /**
     * @brief Create all configured signals for the application.
     * @param app Shared pointer to the application instance.
     * @return Vector of unique pointers to created signals.
     *
     * Creates and returns all signals configured for the application.
     * This includes both toggle and stop signals, configured according
     * to the application's settings.
     */
    [[nodiscard]] virtual auto createSignals(const std::shared_ptr<Application>& app) -> std::vector<std::unique_ptr<ISignal>>;

protected:
    SignalFactory();

private:
    class SignalFactoryImpl;
#pragma warning(push)
#pragma warning(disable: 4251)
    std::unique_ptr<SignalFactoryImpl> pimpl_;
    static std::shared_ptr<SignalFactory> instance_;
#pragma warning(pop)
};

}  // namespace palantir::signal

#endif  // SIGNAL_FACTORY_HPP