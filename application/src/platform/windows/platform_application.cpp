#include "platform_application.hpp"

#include <Windows.h>

#include <cstdint>
#include <memory>
#include <stdexcept>

#include "exception/application_exceptions.hpp"
#include "input/key_codes.hpp"
#include "signal/keyboard_signal_manager.hpp"
#include "utils/logger.hpp"

namespace palantir {

/**
 * @brief Implementation details for the Windows platform application.
 *
 * This class handles the Windows-specific implementation details using the PIMPL idiom.
 * It manages the Windows message loop, keyboard hook, and signal processing infrastructure.
 */
class PlatformApplication::Impl {
public:
    // Delete copy operations
    Impl(const Impl& other) = delete;
    auto operator=(const Impl& other) -> Impl& = delete;
    // Delete move operations (can't move references)
    Impl(Impl&& other) noexcept = delete;
    auto operator=(Impl&& other) noexcept -> Impl& = delete;

    /**
     * @brief Construct the implementation object.
     * @param signalManager Reference to the signal manager for input processing.
     * @param windowManager Reference to the window manager for window handling.
     *
     * Initializes the implementation with references to the managers and sets up
     * the Windows keyboard hook for global input monitoring.
     */
    explicit Impl(const std::shared_ptr<signal::ISignalManager>& signalManager,
                  const std::shared_ptr<window::WindowManager>& windowManager)
        : signalManager_(signalManager), windowManager_(windowManager) {
        DebugLog("Initializing Windows platform application");
    }

    /**
     * @brief Run the Windows application.
     * @return Exit code from the application.
     *
     * Runs the Windows message loop, processing window messages and
     * dispatching them to appropriate window procedures. The loop
     * continues until a WM_QUIT message is received.
     */
    [[nodiscard]] auto run() const -> int {
        DebugLog("Starting message loop");

        MSG msg;

        // Get the main window for regular updates
        auto mainWindow = windowManager_->getMainWindow();
        if (!mainWindow) {
            DebugLog("No main window available");
            return 1;
        }

        // Show the main window
        mainWindow->show();

        // For periodic updates (roughly 60 times per second)
        UINT_PTR timerId = SetTimer(nullptr, 0, 16, nullptr);  // ~60 FPS NOLINT

        // Main message loop
        while (true) {
            // Check if there are messages in the queue
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                // If we get a quit message, break out of the loop
                if (msg.message == WM_QUIT) {
                    break;
                }

                // Process the message
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            } else {
                // No messages, update our window
                if (mainWindow->isRunning()) {
                    mainWindow->update();
                }

                // Don't consume 100% CPU, yield to other threads briefly
                Sleep(1);
            }

            // Process any timer messages
            if (msg.message == WM_TIMER && msg.wParam == timerId && mainWindow->isRunning()) {
                mainWindow->update();
            }
        }

        // Clean up the timer
        if (timerId != 0) {
            KillTimer(nullptr, timerId);
        }

        DebugLog("Message loop ended with exit code %d", static_cast<int>(msg.wParam));
        return static_cast<int>(msg.wParam);
    }

    /**
     * @brief Quit the Windows application.
     *
     * Posts a quit message to the message queue, which will cause
     * the message loop to terminate. Also ensures cleanup of the
     * keyboard hook.
     */
    auto quit() const -> void {
        DebugLog("Application quitting");
        PostQuitMessage(0);
    }

    /**
     * @brief Clean up implementation resources.
     *
     * Ensures proper cleanup of the keyboard hook and global
     * signal manager pointer.
     */
    ~Impl() = default;

private:
    std::shared_ptr<signal::ISignalManager> signalManager_;  ///< Reference to the signal manager
    std::shared_ptr<window::WindowManager> windowManager_;  ///< Reference to the window manager
};

/**
 * @brief Construct the Windows platform application.
 * @param configPath Path to the configuration file.
 *
 * Creates a new Windows platform application instance, initializing the base
 * Application class and creating the platform-specific implementation.
 */
PlatformApplication::PlatformApplication()
    : Application(), pImpl_(std::make_unique<Impl>(getSignalManager(), getWindowManager())) {
    DebugLog("Creating Windows platform application");
}

// Required for unique_ptr with incomplete type
PlatformApplication::~PlatformApplication() = default;

/**
 * @brief Run the Windows application.
 * @return Exit code from the application.
 *
 * Delegates to the implementation's run method to handle the
 * Windows message loop and event processing.
 */
[[nodiscard]] auto PlatformApplication::run() -> int { return pImpl_->run(); }

/**
 * @brief Quit the Windows application.
 *
 * Delegates to the implementation's quit method to handle proper
 * Windows application termination and cleanup.
 */
auto PlatformApplication::quit() -> void { pImpl_->quit(); }

}  // namespace palantir
