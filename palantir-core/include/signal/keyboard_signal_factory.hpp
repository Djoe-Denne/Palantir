/**
 * @file signal_factory.hpp
 * @brief Defines the factory for creating signal handlers.
 *
 * This file contains the SignalFactory class which is responsible for creating
 * signal handlers that connect inputs to commands. It manages the creation of
 * different types of signals based on the application's needs.
 */

#pragma once

#include "signal/isignal_factory.hpp"

#include <memory>
#include <vector>

#include "core_export.hpp"
#include "input/iinput_factory.hpp"

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
class PALANTIR_CORE_API KeyboardSignalFactory : public ISignalFactory {
public:
    /** @brief Constructor. */
    KeyboardSignalFactory();
    KeyboardSignalFactory(const std::shared_ptr<input::IInputFactory>& inputFactory);

    /** @brief Virtual destructor. */
    ~KeyboardSignalFactory() override;

    // Delete copy operations
    /** @brief Deleted copy constructor. */
    KeyboardSignalFactory(const KeyboardSignalFactory&) = delete;
    /** @brief Deleted copy assignment. */
    auto operator=(const KeyboardSignalFactory&) -> KeyboardSignalFactory& = delete;

    // Delete move operations
    /** @brief Deleted move constructor. */
    KeyboardSignalFactory(KeyboardSignalFactory&&) = delete;
    /** @brief Deleted move assignment. */
    auto operator=(KeyboardSignalFactory&&) -> KeyboardSignalFactory& = delete;

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
    class KeyboardSignalFactoryImpl;
#pragma warning(push)
#pragma warning(disable : 4251)
    std::unique_ptr<KeyboardSignalFactoryImpl> pimpl_;
#pragma warning(pop)
};

}  // namespace palantir::signal
