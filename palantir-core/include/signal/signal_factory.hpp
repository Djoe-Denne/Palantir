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

#include "application.hpp"
#include "core_export.hpp"
#include "input/input_factory.hpp"
#include "signal/isignal.hpp"
#include "window/window_manager.hpp"

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
class PALANTIR_CORE_API SignalFactory {
public:
    /** @brief Constructor. */
    SignalFactory();
    SignalFactory(const std::shared_ptr<input::InputFactory>& inputFactory);

    /** @brief Virtual destructor. */
    virtual ~SignalFactory();

    // Delete copy operations
    /** @brief Deleted copy constructor. */
    SignalFactory(const SignalFactory&) = delete;
    /** @brief Deleted copy assignment. */
    auto operator=(const SignalFactory&) -> SignalFactory& = delete;

    // Delete move operations
    /** @brief Deleted move constructor. */
    SignalFactory(SignalFactory&&) = delete;
    /** @brief Deleted move assignment. */
    auto operator=(SignalFactory&&) -> SignalFactory& = delete;

    /**
     * @brief Create all configured signals for the application.
     * @param app Shared pointer to the application instance.
     * @return Vector of unique pointers to created signals.
     *
     * Creates and returns all signals configured for the application.
     * This includes both toggle and stop signals, configured according
     * to the application's settings.
     */
    [[nodiscard]] virtual auto createSignals() const -> std::vector<std::unique_ptr<ISignal>>;

private:
    class SignalFactoryImpl;
#pragma warning(push)
#pragma warning(disable : 4251)
    std::unique_ptr<SignalFactoryImpl> pimpl_;
#pragma warning(pop)
};

}  // namespace palantir::signal

#endif  // SIGNAL_FACTORY_HPP