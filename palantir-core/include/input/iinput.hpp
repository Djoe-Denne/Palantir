/**
 * @file iinput.hpp
 * @brief Defines the input interface for handling keyboard inputs.
 *
 * This file contains the IInput interface which serves as the base for all input
 * handlers in the application. It provides a platform-agnostic way to detect
 * keyboard inputs and modifier key states.
 */

#ifndef IINPUT_HPP
#define IINPUT_HPP

#include <any>

#include "core_export.hpp"

namespace palantir::input {

/**
 * @class IInput
 * @brief Interface for handling keyboard inputs.
 *
 * Provides a common interface for detecting keyboard inputs across different
 * platforms. The interface is designed to handle both regular keys and modifier
 * keys (like Ctrl, Alt, etc.) in a platform-independent way. It supports move
 * semantics but prohibits copying to ensure unique ownership of platform resources.
 */
class PALANTIR_CORE_API IInput {
public:
    /** @brief Virtual destructor to ensure proper cleanup of derived classes. */
    virtual ~IInput() = default;

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent resource duplication. */
    IInput(const IInput&) = delete;
    /** @brief Deleted copy assignment to prevent resource duplication. */
    auto operator=(const IInput&) -> IInput& = delete;

    // Delete move operations
    /** @brief Deleted move constructor to prevent resource duplication. */
    IInput(IInput&&) noexcept = delete;
    /** @brief Deleted move assignment to prevent resource duplication. */
    auto operator=(IInput&&) noexcept -> IInput& = delete;

    /**
     * @brief Check if the configured input is currently active.
     * @return true if the input is active, false otherwise.
     *
     * This method should be implemented to check the current state of the
     * configured input in a platform-specific way. Can be Keys, Mouse, or other input devices. Or touch and gestures
     * for mobile devices.
     */
    [[nodiscard]] virtual auto isActive(const std::any& event) const -> bool = 0;
    /**
     * @brief Update the input handler's state.
     *
     * This method should be implemented to perform any necessary updates
     * to the input handler's state. The implementation may be empty if
     * no state updates are required.
     */
    virtual auto update() -> void = 0;

protected:
    /** @brief Protected default constructor to prevent direct instantiation. */
    IInput() = default;
};

}  // namespace palantir::input

#endif  // IINPUT_HPP