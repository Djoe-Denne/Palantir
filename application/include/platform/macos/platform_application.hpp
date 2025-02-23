/**
 * @file platform_application.hpp
 * @brief Defines the macOS-specific application implementation.
 *
 * This file contains the PlatformApplication class which implements the
 * Application interface for the macOS platform. It provides macOS-specific
 * functionality for application lifecycle and event handling.
 */

#ifndef INTERVIEW_CHEATER_PLATFORM_MACOS_PLATFORM_APPLICATION_HPP
#define INTERVIEW_CHEATER_PLATFORM_MACOS_PLATFORM_APPLICATION_HPP

#include <memory>
#include <string>

#include "application.hpp"
#include "signal/signal_manager.hpp"
#include "window/window_manager.hpp"

namespace interview_cheater {

/**
 * @class PlatformApplication
 * @brief macOS-specific implementation of the application.
 *
 * This class provides the macOS-specific implementation of the Application
 * interface. It handles platform-specific initialization, event processing,
 * and resource management.
 */
class PlatformApplication : public Application {
public:
    /**
     * @brief Construct a new Platform Application object.
     * @param configPath Path to the configuration file.
     *
     * Initializes the macOS application with the specified configuration.
     * Sets up the Cocoa application infrastructure and event monitoring.
     */
    explicit PlatformApplication(const std::string& configPath);

    /** @brief Destroy the Platform Application object. */
    ~PlatformApplication() override;

    // Delete copy operations
    PlatformApplication(const PlatformApplication&) = delete;
    auto operator=(const PlatformApplication&) -> PlatformApplication& = delete;

    // Delete move operations
    PlatformApplication(PlatformApplication&&) noexcept = delete;
    auto operator=(PlatformApplication&&) noexcept -> PlatformApplication& = delete;

    /**
     * @brief Run the macOS application.
     * @return Exit code from the application.
     *
     * Starts the Cocoa run loop and begins processing events.
     * This method will block until the application is terminated.
     */
    [[nodiscard]] auto run() -> int override;

    /**
     * @brief Quit the macOS application.
     *
     * Terminates the Cocoa application and performs cleanup.
     */
    auto quit() -> void override;

private:
    /** @brief Forward declaration of the implementation class. */
    class Impl;
    /** @brief Pointer to the implementation details. */
    std::unique_ptr<Impl> pImpl_;
};

}  // namespace interview_cheater

#endif  // INTERVIEW_CHEATER_PLATFORM_MACOS_PLATFORM_APPLICATION_HPP