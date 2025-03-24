/**
 * @file keyboard_input_signal_factory.hpp
 * @brief Defines the factory for creating keyboard input signal handlers.
 *
 * This file contains the KeyboardInputSignalFactory class which is responsible for creating
 * signal handlers that connect keyboard inputs to commands.
 */

#ifndef KEYBOARD_INPUT_SIGNAL_FACTORY_HPP
#define KEYBOARD_INPUT_SIGNAL_FACTORY_HPP

#include <memory>
#include <vector>

#include "core_export.hpp"
#include "signal/isignal.hpp"
#include "input/input_factory.hpp"
#include "input/keyboard_input_factory.hpp"
#include "config/config.hpp"

namespace palantir::signal {

/**
 * @class KeyboardInputSignalFactory
 * @brief Factory class for creating keyboard input signal handlers.
 *
 * This factory class is responsible for creating and managing signals
 * that connect keyboard inputs to commands. It provides methods to create
 * specific types of signals based on keyboard input configuration.
 */
class PALANTIR_CORE_API KeyboardInputSignalFactory {
public:
    /**
     * @brief Constructor.
     * @param config Configuration for the factory.
     */
    KeyboardInputSignalFactory(const config::Config& config);

    /**
     * @brief Destructor.
     */
    virtual ~KeyboardInputSignalFactory() = default;

    // Delete copy operations
    KeyboardInputSignalFactory(const KeyboardInputSignalFactory&) = delete;
    auto operator=(const KeyboardInputSignalFactory&) -> KeyboardInputSignalFactory& = delete;

    // Delete move operations
    KeyboardInputSignalFactory(KeyboardInputSignalFactory&&) = delete;
    auto operator=(KeyboardInputSignalFactory&&) -> KeyboardInputSignalFactory& = delete;

    /**
     * @brief Create all configured signals for keyboard inputs.
     * @return Vector of unique pointers to created signals.
     *
     * Creates and returns all signals configured for keyboard inputs.
     */
    [[nodiscard]] virtual auto createSignals() const -> std::vector<std::unique_ptr<ISignal>>;

protected:
    KeyboardInputSignalFactory(std::unique_ptr<input::InputFactory<input::KeyboardInputFactory>> inputFactory);

private:
#pragma warning(push)
#pragma warning(disable: 4251)
    // Input factory to create inputs for signals
    std::unique_ptr<input::InputFactory<input::KeyboardInputFactory>> inputFactory_;
#pragma warning(pop)
};

}  // namespace palantir::signal

#endif  // KEYBOARD_INPUT_SIGNAL_FACTORY_HPP 