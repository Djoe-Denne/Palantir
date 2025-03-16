/**
 * @file application.hpp
 * @brief Defines the base application class.
 *
 * This file contains the Application class which serves as the base for
 * platform-specific application implementations. It provides common functionality
 * for application lifecycle, signal management, and window management.
 */

#ifndef PALANTIR_PLATFORM_APPLICATION_HPP
#define PALANTIR_PLATFORM_APPLICATION_HPP

#include <memory>
#include <stdexcept>
#include <string>

#include "core_export.hpp"
#include "signal/signal_manager.hpp"
#include "window/window_manager.hpp"

namespace palantir {

/**
 * @class Application
 * @brief Base class for the application implementation using PIMPL pattern.
 *
 * This class provides the foundation for platform-specific application
 * implementations. It manages the application's lifecycle, handles signals
 * and windows, and provides configuration management. The class follows
 * the singleton pattern to ensure a single application instance.
 */
class PALANTIR_CORE_API Application {
public:
    /**
     * @brief Get the singleton instance of the application.
     * @param configPath Path to the configuration file.
     * @return Shared pointer to the application instance.
     *
     * Returns the singleton instance of the application, creating it if
     * necessary. The instance will be platform-specific (Windows or macOS)
     * based on the compilation target.
     */
    template <typename T>
    static auto getInstance(const std::string& configPath) -> std::shared_ptr<T> {
        if (getInstance() == nullptr) {
            setInstance(std::shared_ptr<T>(new T(configPath)));
        }
        return std::static_pointer_cast<T>(getInstance());
    }

    static auto getInstance() -> std::shared_ptr<Application>;

    static auto setInstance(const std::shared_ptr<Application>& instance) -> void;

    /**
     * @brief Virtual destructor for proper cleanup.
     *
     * Ensures proper cleanup of platform-specific resources.
     */
    virtual ~Application();

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent instance duplication. */
    Application(const Application&) = delete;
    /** @brief Deleted copy assignment to prevent instance duplication. */
    auto operator=(const Application&) -> Application& = delete;

    // Delete move operations
    /** @brief Deleted move constructor since we have non-movable members. */
    Application(Application&&) noexcept = delete;
    /** @brief Deleted move assignment since we have non-movable members. */
    auto operator=(Application&&) noexcept -> Application& = delete;

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
    [[nodiscard]] auto getSignalManager() const -> std::shared_ptr<class signal::SignalManager>;

    /**
     * @brief Get the window manager.
     * @return Reference to the window manager.
     *
     * Returns a reference to the application's window manager, which handles
     * all application windows and their lifecycle.
     */
    [[nodiscard]] auto getWindowManager() const -> std::shared_ptr<class window::WindowManager>;

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
    Application();  // for testing and mocking

private:
    // PIMPL implementation
    class ApplicationImpl;
    // Suppress C4251 warning for this specific line as Impl class is never accessed by client
#pragma warning(push)
#pragma warning(disable : 4251)
    std::unique_ptr<ApplicationImpl> pImpl_;
    /**
     * @brief The singleton instance.
     */
    static std::shared_ptr<Application> instance_;
#pragma warning(pop)
};

}  // namespace palantir

#endif  // PALANTIR_PLATFORM_APPLICATION_HPP