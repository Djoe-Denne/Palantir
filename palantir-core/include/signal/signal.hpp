/**
 * @file signal.hpp
 * @brief Defines the concrete signal implementation.
 *
 * This file contains the Signal class which provides a concrete implementation
 * of the ISignal interface. It connects input handlers with commands and manages
 * the lifecycle of signal processing.
 */

#ifndef INTERVIEW_CHEATER_SIGNAL_SIGNAL_HPP
#define INTERVIEW_CHEATER_SIGNAL_SIGNAL_HPP

#include <any>
#include <chrono>
#include <memory>

#include "signal/isignal.hpp"

namespace interview_cheater {
namespace input {
class IInput;  // Forward declaration
}  // namespace input

namespace command {
class ICommand;  // Forward declaration
}  // namespace command

namespace signal {

/**
 * @class Signal
 * @brief Concrete implementation of the signal interface.
 *
 * This class implements the ISignal interface to provide a complete signal
 * processing system. It connects an input handler with a command and manages
 * the signal's lifecycle, including optional debouncing of rapid inputs.
 */
class Signal final : public ISignal {
public:
    /**
     * @brief Construct a new Signal object.
     * @param input Unique pointer to the input handler.
     * @param command Unique pointer to the command to execute.
     * @param useDebounce Whether to enable input debouncing.
     *
     * Creates a signal that connects the specified input handler with the
     * specified command. Optionally enables debouncing to prevent rapid
     * repeated triggering of the command.
     */
    explicit Signal(std::unique_ptr<input::IInput> input, std::unique_ptr<command::ICommand> command,
                    bool useDebounce = false);

    /** @brief Default destructor. */
    ~Signal() override {
        command_.release();
    }

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent signal duplication. */
    Signal(const Signal&) = delete;
    /** @brief Deleted copy assignment to prevent signal duplication. */
    auto operator=(const Signal&) -> Signal& = delete;

    // Define move operations
    /** @brief Default move constructor for transfer of signal ownership. */
    Signal(Signal&&) noexcept = default;
    /** @brief Default move assignment for transfer of signal ownership. */
    auto operator=(Signal&&) noexcept -> Signal& = default;

    /**
     * @brief Start monitoring for the signal's conditions.
     *
     * Implements the ISignal interface method to begin monitoring for input
     * conditions that should trigger the command. Sets the signal's active
     * state to true.
     */
    auto start() -> void override;

    /**
     * @brief Stop monitoring for the signal's conditions.
     *
     * Implements the ISignal interface method to cease monitoring for input
     * conditions. Sets the signal's active state to false.
     */
    auto stop() -> void override;

    /**
     * @brief Check if the signal is currently active.
     * @return true if the signal is active and monitoring, false otherwise.
     *
     * Implements the ISignal interface method to return the current active
     * state of the signal.
     */
    [[nodiscard]] auto isActive() const -> bool override;

    /**
     * @brief Check if the signal's conditions are met.
     *
     * Implements the ISignal interface method to check the current state of
     * inputs. If the conditions are met and debouncing requirements are
     * satisfied, executes the associated command.
     */
    auto check(const std::any& event) -> void override;

private:
    /** @brief Unique pointer to the input handler. */
    std::unique_ptr<input::IInput> input_;
    /** @brief Unique pointer to the command to execute. */
    std::unique_ptr<command::ICommand> command_;
    /** @brief Current active state of the signal. */
    bool active_{false};
    /** @brief Whether debouncing is enabled for this signal. */
    bool useDebounce_{false};
    /** @brief Timestamp of the last trigger for debouncing. */
    int64_t lastTriggerTime_{0};
    /** @brief Debounce time in nanoseconds (300ms). */
    static constexpr auto DEBOUNCE_TIME =
        std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(300)).count();
};

}  // namespace signal
}  // namespace interview_cheater

#endif  // INTERVIEW_CHEATER_SIGNAL_SIGNAL_HPP