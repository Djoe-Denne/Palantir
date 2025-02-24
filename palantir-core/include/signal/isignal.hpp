/**
 * @file isignal.hpp
 * @brief Defines the signal interface for the signal system.
 *
 * This file contains the ISignal interface which serves as the base for all signals
 * in the application. Signals act as intermediaries between inputs and commands,
 * managing the lifecycle and state of input-triggered actions.
 */

#ifndef ISIGNAL_HPP
#define ISIGNAL_HPP
#include <any>

namespace interview_cheater::signal {

/**
 * @class ISignal
 * @brief Interface for the signal system implementation.
 *
 * Provides a common interface for all signals in the application. Signals are
 * responsible for monitoring input states and triggering appropriate commands
 * when specific conditions are met. The interface supports move semantics but
 * prohibits copying to ensure unique ownership of resources.
 */
class ISignal {
public:
    /** @brief Virtual destructor to ensure proper cleanup of derived classes. */
    virtual ~ISignal() = default;

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent signal duplication. */
    ISignal(const ISignal&) = delete;
    /** @brief Deleted copy assignment to prevent signal duplication. */
    auto operator=(const ISignal&) -> ISignal& = delete;

    // Define move operations
    /** @brief Default move constructor for transfer of signal ownership. */
    ISignal(ISignal&&) noexcept = default;
    /** @brief Default move assignment for transfer of signal ownership. */
    auto operator=(ISignal&&) noexcept -> ISignal& = default;

    /**
     * @brief Start monitoring for the signal's conditions.
     *
     * This method should be implemented to initialize and begin monitoring
     * for the signal's trigger conditions. It may set up event listeners,
     * initialize state variables, or perform other necessary setup.
     */
    virtual auto start() -> void = 0;

    /**
     * @brief Stop monitoring for the signal's conditions.
     *
     * This method should be implemented to cease monitoring for the signal's
     * trigger conditions. It should clean up any resources and ensure the
     * signal no longer responds to inputs.
     */
    virtual auto stop() -> void = 0;

    /**
     * @brief Check if the signal is currently active.
     * @return true if the signal is active and monitoring, false otherwise.
     *
     * Returns the current state of the signal, indicating whether it is
     * actively monitoring for its trigger conditions.
     */
    [[nodiscard]] virtual auto isActive() const -> bool = 0;

    /**
     * @brief Check if the signal's conditions are met.
     *
     * This method should be implemented to check the current state of inputs
     * and determine if the signal's conditions are met. If the conditions
     * are met, it should trigger the appropriate command.
     */
    virtual auto check(const std::any& event) -> void = 0;

protected:
    /** @brief Protected default constructor to prevent direct instantiation. */
    ISignal() = default;
};

}  // namespace interview_cheater::signal

#endif  // ISIGNAL_HPP