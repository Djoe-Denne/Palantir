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
#include "core_export.hpp"
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
class PALANTIR_CORE_API WindowManager : public std::enable_shared_from_this<WindowManager> {
public:
    /**
     * @brief Get the singleton instance of the WindowManager.
     * @return Shared pointer to the WindowManager instance.
     */
    static auto getInstance() -> std::shared_ptr<WindowManager>;

    static auto setInstance(const std::shared_ptr<WindowManager>& instance) -> void;

    /**
     * @brief Destroy the WindowManager object.
     */
    virtual ~WindowManager();

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent window duplication. */
    WindowManager(const WindowManager&) = delete;
    /** @brief Deleted copy assignment to prevent window duplication. */
    auto operator=(const WindowManager&) -> WindowManager& = delete;

    // Delete move operations since we have unique_ptr members
    /** @brief Deleted move constructor since we have unique_ptr members. */
    WindowManager(WindowManager&&) noexcept = delete;
    /** @brief Deleted move assignment since we have unique_ptr members. */
    auto operator=(WindowManager&&) noexcept -> WindowManager& = delete;

    /**
     * @brief Add a new window to the manager.
     * @param window Unique pointer to the window to add.
     *
     * Takes ownership of the provided window and adds it to the collection
     * of managed windows. The window will be managed by this class for its
     * entire lifecycle.
     */
    virtual auto addWindow(std::unique_ptr<IWindow> window) -> void;

    /**
     * @brief Remove a window from the manager.
     * @param window Pointer to the window to remove.
     *
     * Removes the specified window from management and destroys it. The
     * pointer should be one previously added through addWindow.
     */
    virtual auto removeWindow(const IWindow* window) -> void;

    /**
     * @brief Get the first window in the collection.
     * @return Pointer to the first window, or nullptr if no windows exist.
     *
     * Returns a pointer to the first window in the collection. This is
     * typically used when the application only manages a single window.
     */
    [[nodiscard]] virtual auto getFirstWindow() const -> std::shared_ptr<IWindow>;

    /**
     * @brief Check if any windows are currently running.
     * @return true if any windows are running, false otherwise.
     *
     * Returns whether any managed windows are in a running state.
     * This is typically used to determine if the application should
     * continue processing.
     */
    [[nodiscard]] virtual auto hasRunningWindows() const -> bool;

    /**
     * @brief Execute a command on the managed windows.
     * @param command Unique pointer to the command to execute.
     *
     * Takes ownership of the provided command and executes it. The command
     * should be window-related and may affect one or more managed windows.
     */
    virtual auto executeCommand(std::unique_ptr<interview_cheater::command::ICommand> command) -> void;

protected:
    // Protected constructor for testing
    WindowManager();

private:
    // Forward declaration of implementation class
    class WindowManagerImpl;
    // Suppress C4251 warning for this specific line as Impl clas is never accessed by client
#pragma warning(push)
#pragma warning(disable: 4251)
    std::unique_ptr<WindowManagerImpl> pimpl_;
    static std::shared_ptr<WindowManager> instance_;
#pragma warning(pop)

};
}  // namespace window
}  // namespace interview_cheater

#endif  // WINDOW_MANAGER_HPP