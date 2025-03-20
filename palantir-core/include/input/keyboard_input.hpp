/**
 * @file keyboard_Input.hpp
 * @brief Defines the configurable input handler class.
 *
 * This file contains the KeyboardInput class which implements the IInput
 * interface to provide a configurable input handler that can be set up with
 * specific key and modifier codes at runtime.
 */

#ifndef CONFIGURABLE_INPUT_HPP
#define CONFIGURABLE_INPUT_HPP

#include <memory>

#include "core_export.hpp"
#include "input/iinput.hpp"

namespace palantir::input {

/**
 * @class KeyboardInput
 * @brief Configurable implementation of the input handler interface.
 *
 * This class provides a concrete implementation of IInput that can be configured
 * with specific key and modifier codes at runtime. It uses the PIMPL idiom to
 * hide platform-specific implementation details and provide a clean interface.
 */
class PALANTIR_CORE_API KeyboardInput : public IInput {
public:
    /**
     * @brief Construct a new KeyboardInput object.
     * @param keyCode Platform-specific code for the main key.
     * @param modifierCode Platform-specific code for the modifier key.
     *
     * Creates an input handler configured with the specified key and modifier
     * codes. The codes should be obtained through the KeyMapper utility.
     */
    KeyboardInput(int keyCode, int modifierCode);

    /**
     * @brief Destroy the KeyboardInput object.
     *
     * Virtual destructor to ensure proper cleanup of the implementation.
     * Required for proper PIMPL pattern usage with unique_ptr.
     */
    ~KeyboardInput() override;

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent resource duplication. */
    KeyboardInput(const KeyboardInput&) = delete;
    /** @brief Deleted copy assignment to prevent resource duplication. */
    auto operator=(const KeyboardInput&) -> KeyboardInput& = delete;

    // Define move operations
    /** @brief Default move constructor for transfer of input handler ownership. */
    KeyboardInput(KeyboardInput&&) noexcept = delete;
    /** @brief Default move assignment for transfer of input handler ownership. */
    auto operator=(KeyboardInput&&) noexcept -> KeyboardInput& = delete;

    [[nodiscard]] auto isActive(const std::any& event) const -> bool override;

    auto update() -> void override;

private:
    /** @brief Forward declaration of the implementation class. */
    class Impl;
    // Suppress C4251 warning for this specific line as Impl clas is never accessed by client
#pragma warning(push)
#pragma warning(disable : 4251)
    std::unique_ptr<Impl> pImpl_;
#pragma warning(pop)
};

}  // namespace palantir::input

#endif  // CONFIGURABLE_INPUT_HPP