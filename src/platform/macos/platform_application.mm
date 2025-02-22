#include "platform_application.hpp"
#import <Cocoa/Cocoa.h>
#include "input/key_codes.hpp"
#import "utils/logger.hpp"

namespace interview_cheater {

/**
 * @brief Implementation details for the macOS platform application.
 *
 * This class handles the macOS-specific implementation details using the PIMPL idiom.
 * It manages the Cocoa application lifecycle and event monitoring infrastructure.
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
     * the Cocoa application infrastructure. Also requests accessibility permissions
     * which are required for global event monitoring.
     */
    explicit Impl(signal::SignalManager& signalManager, window::WindowManager& windowManager)
        : signalManager_(signalManager), windowManager_(windowManager) {
        DEBUG_LOG("Initializing PlatformApplication");
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyAccessory];

        // Request accessibility permissions if needed
        NSDictionary* options = @{(__bridge id)kAXTrustedCheckOptionPrompt : @YES};
        bool accessibilityEnabled = AXIsProcessTrustedWithOptions((CFDictionaryRef)options) != 0;

        DEBUG_LOG("Accessibility status: %s", accessibilityEnabled ? "Enabled" : "Disabled");

        if (!accessibilityEnabled) {
            NSString* message = @"Please grant accessibility permissions in System Preferences > "
                                @"Security & Privacy > Privacy > Accessibility";
            DEBUG_LOG("%s", [message UTF8String]);
        }

        DEBUG_LOG("Application initialization complete");
    }

    /**
     * @brief Run the macOS application.
     * @return Exit code from the application.
     *
     * Starts the Cocoa run loop, which will continue until the application
     * is terminated. This is the main event processing loop for macOS.
     */
    [[nodiscard]] auto run() -> int {
        DEBUG_LOG("Starting application run loop");
        [NSApp run];
        return 0;
    }

    /**
     * @brief Quit the macOS application.
     *
     * Terminates the Cocoa application and cleans up resources.
     */
    auto quit() -> void {
        DEBUG_LOG("Application quitting");
        [NSApp terminate:nil];
    }

   private:
    signal::SignalManager& signalManager_;  ///< Reference to the signal manager
    window::WindowManager& windowManager_;  ///< Reference to the window manager
};

PlatformApplication::PlatformApplication(const std::string& configPath)
    : Application(configPath), pImpl_(std::make_unique<Impl>(getSignalManager(), getWindowManager())) {
    DEBUG_LOG("Creating MacOS platform application");
}

// Required for unique_ptr with incomplete type
PlatformApplication::~PlatformApplication() = default;

[[nodiscard]] auto PlatformApplication::run() -> int { return pImpl_->run(); }

auto PlatformApplication::quit() -> void { pImpl_->quit(); }

}  // namespace interview_cheater