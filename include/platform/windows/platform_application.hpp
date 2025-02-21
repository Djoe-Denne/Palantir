/**
 * @file platform_application.hpp
 * @brief Defines the Windows-specific application implementation.
 *
 * This file contains the PlatformApplication class which implements the
 * Application interface for the Windows platform. It provides Windows-specific
 * functionality for application lifecycle and event handling.
 */

#ifndef WINDOWS_APPLICATION_HPP
#define WINDOWS_APPLICATION_HPP

#include <memory>

#include "platform/application.hpp"

namespace interview_cheater {
namespace signal {
// Forward declaration for SignalManager
class SignalManager;
}  // namespace signal

/**
 * @class PlatformApplication
 * @brief Windows-specific implementation of the Application class.
 *
 * This class provides the Windows platform implementation of the application.
 * It handles Windows-specific event processing, window management, and
 * application lifecycle. The implementation uses the Windows API for
 * native functionality.
 */
class PlatformApplication : public Application {
public:
    /**
     * @brief Construct a new PlatformApplication object.
     * @param configPath Path to the configuration file.
     *
     * Creates a Windows-specific application instance, initializing Windows
     * API resources and setting up the message loop infrastructure.
     */
    explicit PlatformApplication(const std::string& configPath);

    /**
     * @brief Destroy the PlatformApplication object.
     *
     * Cleans up Windows-specific resources and ensures proper shutdown
     * of the Windows API components.
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
     * @brief Run the Windows application.
     * @return Exit code from the application.
     *
     * Implements the main Windows message loop, processing Windows events
     * and dispatching them to appropriate handlers. Returns when the
     * application receives a quit message.
     */
    auto run() -> int override;

    /**
     * @brief Quit the Windows application.
     *
     * Posts a quit message to the Windows message queue, triggering a
     * clean shutdown of the application.
     */
    auto quit() -> void override;

private:
    /** @brief Forward declaration of the implementation class. */
    class Impl;
    /** @brief Pointer to the implementation details. */
    std::unique_ptr<Impl> pImpl_;
};
}  // namespace interview_cheater

#endif  // WINDOWS_APPLICATION_HPP