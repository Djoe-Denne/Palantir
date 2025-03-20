/**
 * @file keyboard_input_signal_factory.hpp
 * @brief Defines a factory for creating keyboard input signal handlers.
 *
 * This file contains the KeyboardInputSignalFactory class which is responsible
 * for creating signal handlers that connect keyboard inputs to commands.
 */

#ifndef KEYBOARD_INPUT_SIGNAL_FACTORY_HPP
#define KEYBOARD_INPUT_SIGNAL_FACTORY_HPP

#include <memory>
#include <vector>
#include <string>

#include "command/command_factory.hpp"
#include "command/icommand.hpp"
#include "core_export.hpp"
#include "exception/exceptions.hpp"
#include "input/input_factory.hpp"
#include "input/iinput.hpp"
#include "signal/input_signal.hpp"
#include "signal/isignal.hpp"
#include "utils/logger.hpp"

namespace palantir::signal {

/**
 * @class KeyboardInputSignalFactory
 * @brief Factory class for creating keyboard input signal handlers.
 *
 * This factory class is responsible for creating and managing signal
 * handlers that connect keyboard inputs to commands.
 */
class PALANTIR_CORE_API KeyboardInputSignalFactory {
public:
    KeyboardInputSignalFactory() = default;
    virtual ~KeyboardInputSignalFactory() = default;

    // Delete copy operations
    KeyboardInputSignalFactory(const KeyboardInputSignalFactory&) = delete;
    auto operator=(const KeyboardInputSignalFactory&) -> KeyboardInputSignalFactory& = delete;

    // Delete move operations
    KeyboardInputSignalFactory(KeyboardInputSignalFactory&&) = delete;
    auto operator=(KeyboardInputSignalFactory&&) -> KeyboardInputSignalFactory& = delete;

    /**
     * @brief Create all configured keyboard input signals.
     * @return Vector of unique pointers to created signals.
     *
     * Creates and returns all keyboard input signals configured for the application.
     */
    [[nodiscard]] auto createSignals() const -> std::vector<std::unique_ptr<ISignal>>;
};

}  // namespace palantir::signal

#endif  // KEYBOARD_INPUT_SIGNAL_FACTORY_HPP 