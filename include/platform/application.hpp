/**
 * @file application.hpp
 * @brief Defines the base application class.
 *
 * This file contains the Application class which serves as the base for
 * platform-specific application implementations. It provides common functionality
 * for application lifecycle, signal management, and window management.
 */

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>
#include <string>

#include "signal/signal_manager.hpp"
#include "window/window_manager.hpp"

namespace interview_cheater {

/**
 * @class Application
 * @brief Base class for the application implementation.
 *
 * This class provides the foundation for platform-specific application
 * implementations. It manages the application's lifecycle, handles signals
 * and windows, and provides configuration management. The class follows
 * the singleton pattern to ensure a single application instance.
 */
class Application {
public:
    /**
     * @brief Get the singleton instance of the application.
     * @param configPath Path to the configuration file.
     * @return Pointer to the application instance.
     *
     * Returns the singleton instance of the application, creating it if
     * necessary. The instance will be platform-specific (Windows or macOS)
     * based on the compilation target.
     */
    static auto getInstance(const std::string& configPath) -> Application*;

    /**
     * @brief Virtual destructor for proper cleanup.
     *
     * Ensures proper cleanup of platform-specific resources.
     */
    virtual ~Application() = default;

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent instance duplication. */
    Application(const Application&) = delete;
    /** @brief Deleted copy assignment to prevent instance duplication. */
    auto operator=(const Application&) -> Application& = delete;

    // Define move operations
    /** @brief Default move constructor for transfer of application ownership. */
    Application(Application&&) noexcept = default;
    /** @brief Default move assignment for transfer of application ownership. */
    auto operator=(Application&&) noexcept -> Application& = default;

    /**
     * @brief Run the application.
     * @return Exit code from the application.
     *
     * Platform-specific implementation of the application's main run loop.
     * This method should handle the event loop and return when the
     * application is ready to exit.
     */
    [[nodiscard]] virtual auto run() -> int = 0;

    /**
     * @brief Quit the application.
     *
     * Platform-specific implementation of application shutdown. This method
     * should trigger a clean exit of the application.
     */
    virtual auto quit() -> void = 0;

    /**
     * @brief Get the signal manager.
     * @return Reference to the signal manager.
     *
     * Returns a reference to the application's signal manager, which handles
     * all input signals and their processing.
     */
    [[nodiscard]] auto getSignalManager() -> signal::SignalManager& { return signalManager_; }

    /**
     * @brief Get the window manager.
     * @return Reference to the window manager.
     *
     * Returns a reference to the application's window manager, which handles
     * all application windows and their lifecycle.
     */
    [[nodiscard]] auto getWindowManager() -> window::WindowManager& { return windowManager_; }

    /**
     * @brief Initialize signals from configuration.
     *
     * Loads and attaches signals based on the application's configuration.
     * This includes creating appropriate signal handlers and connecting
     * them to commands.
     */
    auto attachSignals() -> void;

protected:
    /**
     * @brief Construct a new Application object.
     * @param configPath Path to the configuration file.
     *
     * Protected constructor to enforce singleton pattern. Initializes
     * the application with the specified configuration path.
     */
    explicit Application(const std::string& configPath);

private:
    /** @brief Singleton instance pointer. */
    static Application* instance_;
    /** @brief Path to the configuration file. */
    const std::string configPath_;
    /** @brief Window manager instance. */
    window::WindowManager windowManager_;
    /** @brief Signal manager instance. */
    signal::SignalManager signalManager_;
};

}  // namespace interview_cheater

#endif  // APPLICATION_HPP