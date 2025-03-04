/**
 * @file iwindow.hpp
 * @brief Defines the window interface for the application's UI components.
 *
 * This file contains the IWindow interface which serves as the base for all window
 * implementations in the application. It provides a platform-agnostic way to manage
 * window lifecycle and state.
 */

#ifndef IWINDOW_HPP
#define IWINDOW_HPP

#include <memory>
#include "core_export.hpp"
#include "window/component/icontent_manager.hpp"

namespace interview_cheater::window {

/**
 * @class IWindow
 * @brief Interface for window management.
 *
 * Provides a common interface for all windows in the application. Windows are
 * responsible for managing their lifecycle, visibility, and state. The interface
 * supports move semantics but prohibits copying to ensure unique ownership of
 * window resources.
 */
class PALANTIR_CORE_API IWindow {
public:
    /** @brief Virtual destructor to ensure proper cleanup of derived classes. */
    virtual ~IWindow() = default;

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent window duplication. */
    IWindow(const IWindow&) = delete;
    /** @brief Deleted copy assignment to prevent window duplication. */
    auto operator=(const IWindow&) -> IWindow& = delete;

    // Define move operations
    /** @brief Default move constructor for transfer of window ownership. */
    IWindow(IWindow&&) noexcept = default;
    /** @brief Default move assignment for transfer of window ownership. */
    auto operator=(IWindow&&) noexcept -> IWindow& = default;

    /**
     * @brief Create and initialize the window.
     *
     * This method should be implemented to perform the initial creation and
     * setup of the window. This includes creating the native window handle
     * and setting up any necessary resources.
     */
    virtual auto create() -> void = 0;

    /**
     * @brief Show or toggle the window's visibility.
     *
     * This method should be implemented to handle showing the window or
     * toggling its visibility state. The exact behavior may depend on
     * the window's current state and platform-specific requirements.
     */
    virtual auto show() -> void = 0;

    /**
     * @brief Update the window's state and content.
     *
     * This method should be implemented to handle any necessary updates
     * to the window's state or content. This might include redrawing,
     * processing messages, or updating animations.
     */
    virtual auto update() -> void = 0;

    /**
     * @brief Close the window and clean up resources.
     *
     * This method should be implemented to handle proper window closure
     * and cleanup of any associated resources. This includes destroying
     * the native window handle and freeing any allocated memory.
     */
    virtual auto close() -> void = 0;

    /**
     * @brief Set the window's transparency.
     * @param transparency The transparency value to set (0-100).
     */
    virtual auto setTransparency(int transparency) -> void = 0; 

    /**
     * @brief Toggle the window's anonymity. Preventing any capture or screen sharing.
     */
    virtual auto toggleWindowAnonymity() -> void = 0;

    /**
     * @brief Check if the window is currently running.
     * @return true if the window is running, false otherwise.
     *
     * Returns the current running state of the window, indicating whether
     * it is active and processing events.
     */
    [[nodiscard]] virtual auto isRunning() const -> bool = 0;

    /**
     * @brief Set the window's running state.
     * @param runningState The new running state to set.
     *
     * This method should be implemented to update the window's running state
     * and perform any necessary actions based on the state change.
     */
    virtual auto setRunning(bool runningState) -> void = 0;

    /**
     * @brief Get the native window handle.
     * @return A void pointer to the native window handle.
     *
     * Returns a platform-specific window handle cast to void*. The actual
     * implementation will cast this to the appropriate type (e.g., HWND
     * for Windows or NSWindow* for macOS).
     */
    [[nodiscard]] virtual auto getNativeHandle() const -> void* = 0;

    /**
     * @brief Get the window's content.
     * @return A string representing the window's content.
     */
    [[nodiscard]] virtual auto getContentManager() const -> std::shared_ptr<component::IContentManager> = 0;
protected:
    /** @brief Protected default constructor to prevent direct instantiation. */
    IWindow() = default;
};

}  // namespace interview_cheater::window

#endif  // IWINDOW_HPP