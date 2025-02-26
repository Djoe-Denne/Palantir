/**
 * @file configurable_input.hpp
 * @brief Defines the configurable input handler class.
 *
 * This file contains the ConfigurableInput class which implements the IInput
 * interface to provide a configurable input handler that can be set up with
 * specific key and modifier codes at runtime.
 */

#ifndef CONFIGURABLE_INPUT_HPP
#define CONFIGURABLE_INPUT_HPP

#include <memory>

#include "input/iinput.hpp"

namespace interview_cheater::input {

/**
 * @class ConfigurableInput
 * @brief Configurable implementation of the input handler interface.
 *
 * This class provides a concrete implementation of IInput that can be configured
 * with specific key and modifier codes at runtime. It uses the PIMPL idiom to
 * hide platform-specific implementation details and provide a clean interface.
 */
class ConfigurableInput : public IInput {
public:
    /**
     * @brief Construct a new ConfigurableInput object.
     * @param keyCode Platform-specific code for the main key.
     * @param modifierCode Platform-specific code for the modifier key.
     *
     * Creates an input handler configured with the specified key and modifier
     * codes. The codes should be obtained through the KeyMapper utility.
     */
    ConfigurableInput(int keyCode, int modifierCode);

    /**
     * @brief Destroy the ConfigurableInput object.
     *
     * Virtual destructor to ensure proper cleanup of the implementation.
     * Required for proper PIMPL pattern usage with unique_ptr.
     */
    ~ConfigurableInput() override;

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent resource duplication. */
    ConfigurableInput(const ConfigurableInput&) = delete;
    /** @brief Deleted copy assignment to prevent resource duplication. */
    auto operator=(const ConfigurableInput&) -> ConfigurableInput& = delete;

    // Define move operations
    /** @brief Default move constructor for transfer of input handler ownership. */
    ConfigurableInput(ConfigurableInput&&) noexcept = default;
    /** @brief Default move assignment for transfer of input handler ownership. */
    auto operator=(ConfigurableInput&&) noexcept -> ConfigurableInput& = default;

    /**
     * @brief Check if the configured key is currently pressed.
     * @return true if the key is pressed, false otherwise.
     *
     * Implements the IInput interface method to check the state of the
     * configured key using platform-specific mechanisms.
     */
    [[nodiscard]] auto isKeyPressed(const std::any& event) const -> bool override;

    /**
     * @brief Check if the configured modifier key is currently active.
     * @return true if the modifier is active, false otherwise.
     *
     * Implements the IInput interface method to check the state of the
     * configured modifier key using platform-specific mechanisms.
     */
    [[nodiscard]] auto isModifierActive(const std::any& event) const -> bool override;

    /**
     * @brief Update the input handler's state.
     *
     * Implements the IInput interface method to perform any necessary
     * updates to the input handler's state. The implementation may be
     * empty if no state updates are required.
     */
    auto update() -> void override;

private:
    /** @brief Forward declaration of the implementation class. */
    class Impl;
    // Suppress C4251 warning for this specific line as Impl clas is never accessed by client
#pragma warning(push)
#pragma warning(disable: 4251)
    std::unique_ptr<Impl> pImpl_;
#pragma warning(pop)
};

}  // namespace interview_cheater::input

#endif  // CONFIGURABLE_INPUT_HPP