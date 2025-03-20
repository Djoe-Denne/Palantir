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
#include <tuple>
#include <utility>

#include "core_export.hpp"
#include "signal/isignal.hpp"

namespace palantir::signal {

/**
 * @class SignalFactory
 * @brief Factory class for creating signal handlers.
 *
 * This template factory class is responsible for creating and managing signal
 * handlers that connect inputs to commands. It provides methods to create
 * specific types of signals as well as collections of signals based on
 * configuration. The class accepts a list of concrete factory types as template parameters.
 * 
 * @tparam ConcreteFactories The concrete factory types that will be used to create signals
 */
template<typename... ConcreteFactories>
class PALANTIR_CORE_API SignalFactory {
public:
    virtual ~SignalFactory() = default;

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
    [[nodiscard]] static auto getInstance() -> std::shared_ptr<SignalFactory> {
        if (!instance_) {
            instance_ = std::shared_ptr<SignalFactory>(new SignalFactory());
        }
        return instance_;
    }

    /**
     * @brief Set the singleton instance of the SignalFactory.
     * @param instance A shared pointer to the SignalFactory instance.
     *
     * Sets the singleton instance of the SignalFactory.
     */
    static auto setInstance(const std::shared_ptr<SignalFactory>& instance) -> void {
        instance_ = instance;
    }

    /**
     * @brief Create all configured signals for the application.
     * @return Vector of unique pointers to created signals.
     *
     * Creates and returns all signals configured for the application by calling
     * createSignals() on each concrete factory.
     */
    [[nodiscard]] virtual auto createSignals() const -> std::vector<std::unique_ptr<ISignal>> {
        std::vector<std::unique_ptr<ISignal>> signals;
        
        // Call createSignals on each concrete factory and append to results
        (appendSignals<ConcreteFactories>(signals), ...);
        
        return signals;
    }

protected:
    SignalFactory() : factories_(std::make_tuple(std::make_unique<ConcreteFactories>()...)) {}

private:
#pragma warning(push)
#pragma warning(disable: 4267)
    std::tuple<std::unique_ptr<ConcreteFactories>...> factories_;

    template<typename Factory>
    void appendSignals(std::vector<std::unique_ptr<ISignal>>& signals) const {
        auto factory_signals = std::get<std::unique_ptr<Factory>>(factories_)->createSignals();
        signals.insert(signals.end(),
                       std::make_move_iterator(factory_signals.begin()),
                       std::make_move_iterator(factory_signals.end()));
    }

    static std::shared_ptr<SignalFactory> instance_;
#pragma warning(pop)
};

// Initialize the static instance
template<typename... ConcreteFactories>
std::shared_ptr<SignalFactory<ConcreteFactories...>> SignalFactory<ConcreteFactories...>::instance_;

}  // namespace palantir::signal

#endif  // SIGNAL_FACTORY_HPP