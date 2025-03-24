/**
 * @file signal_factory.hpp
 * @brief Defines the factory for creating signal handlers.
 *
 * This file contains the SignalFactory template class which is responsible for creating
 * signal handlers that connect inputs to commands. It manages the creation of
 * different types of signals based on the application's needs.
 */

#ifndef SIGNAL_FACTORY_HPP
#define SIGNAL_FACTORY_HPP

#include <memory>
#include <tuple>
#include <vector>

#include "core_export.hpp"
#include "signal/isignal.hpp"
#include "concept/signal_concept.hpp"
#include "command/command_factory.hpp"
#include "exception/exceptions.hpp"
#include "utils/logger.hpp"
#include "config/config.hpp"

namespace palantir::signal {

/**
 * @class SignalFactory
 * @brief Templated factory class for managing concrete signal handlers factories.
 *
 * This factory class is responsible for managing concrete signal factories.
 * It provides a centralized way to access different signal factory implementations.
 * @tparam ConcreteFactories A list of concrete factory classes that will be used to create signals.
 */
template<SIGNAL_FACTORY_TEMPLATE... ConcreteFactories>
class PALANTIR_CORE_API SignalFactory {
    // Check that at least one concrete factory is provided
    static_assert(sizeof...(ConcreteFactories) > 0, "At least one concrete factory must be provided");

public:
    /**
     * @brief Constructor that initializes all concrete factories.
     */
    SignalFactory(const config::Config& config) : factories_(std::make_tuple(std::make_unique<ConcreteFactories>(config)...)) {
    }

    /**
     * @brief Destructor.
     */
    virtual ~SignalFactory() = default;

    // Delete copy operations
    SignalFactory(const SignalFactory&) = delete;
    auto operator=(const SignalFactory&) -> SignalFactory& = delete;

    // Delete move operations
    SignalFactory(SignalFactory&&) = delete;
    auto operator=(SignalFactory&&) -> SignalFactory& = delete;

    /**
     * @brief Create all configured signals for the application.
     * @return Vector of unique pointers to created signals.
     *
     * Creates and returns all signals configured for the application.
     * This method aggregates signals from all concrete factories.
     */
    [[nodiscard]] virtual auto createSignals() const -> std::vector<std::unique_ptr<ISignal>> {
        std::vector<std::unique_ptr<ISignal>> signals;
        // Call createSignals on each factory and merge the results
        std::apply([&signals](const auto&... factoryPtrs) {
            std::vector<std::unique_ptr<ISignal>> tempSignals;
            ((tempSignals = factoryPtrs->createSignals(), 
              signals.insert(signals.end(), 
                           std::make_move_iterator(tempSignals.begin()), 
                           std::make_move_iterator(tempSignals.end()))), ...);
        }, factories_);
        
        return signals;
    }

private:
#pragma warning(push)
#pragma warning(disable: 4267)
    // Tuple of unique pointers to all concrete factories
    std::tuple<std::unique_ptr<ConcreteFactories>...> factories_;
#pragma warning(pop)
};

}  // namespace palantir::signal

#endif  // SIGNAL_FACTORY_HPP