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

namespace interview_cheater {
class Application;  // Forward declaration
namespace window {
class WindowManager;  // Forward declaration
}
}  // namespace interview_cheater

namespace interview_cheater::signal {

/**
 * @class SignalFactory
 * @brief Factory class for creating signal handlers.
 *
 * This static factory class is responsible for creating and managing signal
 * handlers that connect inputs to commands. It provides methods to create
 * specific types of signals as well as collections of signals based on
 * configuration. The class is non-instantiable and provides only static methods.
 */
class SignalFactory {
public:
    /** @brief Deleted constructor to prevent instantiation. */
    SignalFactory() = delete;
    /** @brief Deleted destructor to prevent instantiation. */
    ~SignalFactory() = delete;

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
     * @brief Create all configured signals for the application.
     * @param app Reference to the application instance.
     * @return Vector of unique pointers to created signals.
     *
     * Creates and returns all signals configured for the application.
     * This includes both toggle and stop signals, configured according
     * to the application's settings.
     */
    static auto createSignals(Application& app) -> std::vector<std::unique_ptr<ISignal>>;
};

}  // namespace interview_cheater::signal

#endif  // SIGNAL_FACTORY_HPP