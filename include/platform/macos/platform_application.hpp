/**
 * @file platform_application.hpp
 * @brief Defines the macOS-specific application implementation.
 * 
 * This file contains the PlatformApplication class which implements the
 * Application interface for the macOS platform. It provides macOS-specific
 * functionality for application lifecycle and event handling.
 */

#ifndef MACOS_APPLICATION_HPP
#define MACOS_APPLICATION_HPP

#include "platform/application.hpp"
#include "signal/signal_manager.hpp"

namespace interview_cheater {

/**
 * @class PlatformApplication
 * @brief macOS-specific implementation of the Application class.
 * 
 * This class provides the macOS platform implementation of the application.
 * It handles macOS-specific event processing, window management, and
 * application lifecycle. The implementation uses Cocoa and Core Foundation
 * for native functionality.
 */
class PlatformApplication : public Application {
public:
    /**
     * @brief Construct a new PlatformApplication object.
     * @param configPath Path to the configuration file.
     * 
     * Creates a macOS-specific application instance, initializing Cocoa
     * resources and setting up the event monitoring infrastructure.
     * Also requests necessary permissions for accessibility features.
     */
    explicit PlatformApplication(const std::string& configPath);

    /**
     * @brief Destroy the PlatformApplication object.
     * 
     * Cleans up macOS-specific resources and ensures proper shutdown
     * of the Cocoa components and event monitors.
     */
    ~PlatformApplication() override;

    // Delete copy operations
    /** @brief Deleted copy constructor to prevent instance duplication. */
    PlatformApplication(const PlatformApplication&) = delete;
    /** @brief Deleted copy assignment to prevent instance duplication. */
    auto operator=(const PlatformApplication&) -> PlatformApplication& = delete;

    // Define move operations
    /** @brief Default move constructor for transfer of application ownership. */
    PlatformApplication(PlatformApplication&&) noexcept = default;
    /** @brief Default move assignment for transfer of application ownership. */
    auto operator=(PlatformApplication&&) noexcept -> PlatformApplication& = default;

    /**
     * @brief Run the macOS application.
     * @return Exit code from the application.
     * 
     * Implements the main Cocoa run loop, processing macOS events
     * and dispatching them to appropriate handlers. Returns when the
     * application terminates.
     */
    auto run() -> int override;

    /**
     * @brief Quit the macOS application.
     * 
     * Terminates the Cocoa application, triggering a clean shutdown
     * of the application and cleanup of resources.
     */
    auto quit() -> void override;

private:
    /** @brief Forward declaration of the implementation class. */
    class Impl;
    /** @brief Pointer to the implementation details. */
    std::unique_ptr<Impl> pImpl_;
};

}  // namespace interview_cheater

#endif  // MACOS_APPLICATION_HPP