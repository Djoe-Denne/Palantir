/**
 * @file window_manager.hpp
 * @brief Defines the window management system.
 *
 * This file contains the WindowManager class which is responsible for managing
 * all windows in the application. It handles window lifecycle, visibility,
 * and command execution.
 */

#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP

#include <memory>
#include <vector>

#include "window/iwindow.hpp"
namespace interview_cheater {
namespace command {
// Forward declaration for ICommand
class ICommand;
}  // namespace command

namespace window {

/**
 * @class WindowManager
 * @brief Manager class for handling multiple windows.
 *
 * This class manages a collection of windows, controlling their lifecycle
 * and state. It provides methods to add and remove windows, access windows,
 * and execute window-related commands. The class supports move semantics
 * but prohibits copying to ensure unique ownership of windows.
 */
class WindowManager {
public:
    /**
     * @brief Construct a new WindowManager object.
     *
     * Initializes an empty window manager ready to handle windows.
     */
    WindowManager() = default;

    /**
     * @brief Destroy the WindowManager object.
     *
     * Cleans up all managed windows, ensuring proper resource cleanup.
     */
    ~WindowManager() = default;

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent window duplication. */
    WindowManager(const WindowManager&) = delete;
    /** @brief Deleted copy assignment to prevent window duplication. */
    auto operator=(const WindowManager&) -> WindowManager& = delete;

    // Define move operations
    /** @brief Default move constructor for transfer of manager ownership. */
    WindowManager(WindowManager&&) = delete;
    /** @brief Default move assignment for transfer of manager ownership. */
    auto operator=(WindowManager&&) -> WindowManager& = delete;

    /**
     * @brief Add a new window to the manager.
     * @param window Unique pointer to the window to add.
     *
     * Takes ownership of the provided window and adds it to the collection
     * of managed windows. The window will be managed by this class for its
     * entire lifecycle.
     */
    auto addWindow(std::unique_ptr<IWindow> window) -> void;

    /**
     * @brief Remove a window from the manager.
     * @param window Pointer to the window to remove.
     *
     * Removes the specified window from management and destroys it. The
     * pointer should be one previously added through addWindow.
     */
    auto removeWindow(const IWindow* window) -> void;

    /**
     * @brief Get the first window in the collection.
     * @return Pointer to the first window, or nullptr if no windows exist.
     *
     * Returns a pointer to the first window in the collection. This is
     * typically used when the application only manages a single window.
     */
    [[nodiscard]] auto getFirstWindow() const -> IWindow*;

    /**
     * @brief Check if any windows are currently running.
     * @return true if any windows are running, false otherwise.
     *
     * Returns whether any managed windows are in a running state.
     * This is typically used to determine if the application should
     * continue processing.
     */
    [[nodiscard]] auto hasRunningWindows() const -> bool;

    /**
     * @brief Execute a command on the managed windows.
     * @param command Unique pointer to the command to execute.
     *
     * Takes ownership of the provided command and executes it. The command
     * should be window-related and may affect one or more managed windows.
     */
    auto executeCommand(std::unique_ptr<interview_cheater::command::ICommand> command) -> void;

private:
    /** @brief Collection of managed windows. */
    std::vector<std::unique_ptr<IWindow>> windows_;
};
}  // namespace window
}  // namespace interview_cheater

#endif  // WINDOW_MANAGER_HPP