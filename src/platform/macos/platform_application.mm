#include "platform_application.hpp"
#import <Cocoa/Cocoa.h>
#include "input/key_codes.hpp"
#import "utils/logger.hpp"

/**
 * @brief Signal checking class for macOS event monitoring.
 * 
 * This Objective-C class handles the event monitoring for keyboard inputs
 * on macOS. It sets up both global and local monitors to capture keyboard
 * events and trigger signal checks.
 */
@interface SignalChecker : NSObject
@property(nonatomic, assign) interview_cheater::signal::SignalManager* signalManager;
@property(nonatomic, strong) id globalMonitor;
@property(nonatomic, strong) id localMonitor;
- (instancetype)initWithSignalManager:(interview_cheater::signal::SignalManager*)signalManager;
@end

@implementation SignalChecker

/**
 * @brief Initialize the signal checker.
 * @param signalManager Pointer to the C++ signal manager.
 * @return Initialized instance or nil on failure.
 * 
 * Creates a new signal checker instance and sets up the event monitors
 * for keyboard input detection.
 */
- (instancetype)initWithSignalManager:(interview_cheater::signal::SignalManager*)signalManager {
    if ((self = [super init]) != nil) {
        DEBUG_LOG("Initializing SignalChecker");
        self.signalManager = signalManager;
        [self setupEventMonitors];
    }
    return self;
}

/**
 * @brief Set up the keyboard event monitors.
 * 
 * Creates both global and local event monitors to capture keyboard events:
 * - Global monitor: captures events when app is not active
 * - Local monitor: captures events when app is active
 * Both monitors watch for key down and modifier flag changes.
 */
- (void)setupEventMonitors {
    DEBUG_LOG("Setting up event monitors");
    NSEventMask eventMask = NSEventMaskKeyDown | NSEventMaskFlagsChanged;

    // Global monitor for when app is not active
    self.globalMonitor =
        [NSEvent addGlobalMonitorForEventsMatchingMask:eventMask
                                               handler:^(NSEvent* event) {
                                                   DEBUG_LOG("Global event received: type=%lu, "
                                                             "keyCode=%d, modifiers=0x%lx",
                                                             (unsigned long)event.type, (int)event.keyCode,
                                                             (unsigned long)event.modifierFlags);
                                                   [self handleKeyEvent:event];
                                               }];

    // Local monitor for when app is active
    self.localMonitor =
        [NSEvent addLocalMonitorForEventsMatchingMask:eventMask
                                              handler:^NSEvent*(NSEvent* event) {
                                                  DEBUG_LOG("Local event received: type=%lu, "
                                                            "@keyCode=%d, modifiers=0x%lx",
                                                            (unsigned long)event.type, (int)event.keyCode,
                                                            (unsigned long)event.modifierFlags);
                                                  [self handleKeyEvent:event];
                                                  return event;
                                              }];

    if (self.globalMonitor == nil || self.localMonitor == nil) {
        DEBUG_LOG("Warning: Failed to create one or more event monitors");
    }
}

/**
 * @brief Handle a keyboard event.
 * @param event The NSEvent containing keyboard event information.
 * 
 * Processes keyboard events by dispatching a signal check on the main queue.
 * This ensures thread safety when triggering signal checks.
 */
- (void)handleKeyEvent:(NSEvent*)event {
    // Dispatch signal check to main queue for thread safety
    dispatch_async(dispatch_get_main_queue(), ^{
        DEBUG_LOG("Triggering signal check");
        self.signalManager->checkSignals();
    });
}

/**
 * @brief Clean up event monitors.
 * 
 * Removes both global and local event monitors when the instance
 * is being deallocated.
 */
- (void)dealloc {
    DEBUG_LOG("Cleaning up SignalChecker");
    if (self.globalMonitor != nil) {
        [NSEvent removeMonitor:self.globalMonitor];
    }
    if (self.localMonitor != nil) {
        [NSEvent removeMonitor:self.localMonitor];
    }
}

@end

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
        DEBUG_LOG("Initializing ");
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

        signalChecker_ = [[SignalChecker alloc] initWithSignalManager:&signalManager];
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
     * Terminates the Cocoa application and cleans up the signal checker.
     * This ensures proper cleanup of event monitors and other resources.
     */
    auto quit() -> void {
        DEBUG_LOG("Application quitting");
        if (signalChecker_ != nil) {
            signalChecker_ = nil;
        }
        [NSApp terminate:nil];
    }

    /**
     * @brief Clean up implementation resources.
     * 
     * Ensures proper cleanup of the signal checker and its associated
     * event monitors.
     */
    ~Impl() {
        DEBUG_LOG("Application being destroyed");
        if (signalChecker_ != nil) {
            signalChecker_ = nil;
        }
    }

   private:
    signal::SignalManager& signalManager_;  ///< Reference to the signal manager
    window::WindowManager& windowManager_;   ///< Reference to the window manager
    SignalChecker* signalChecker_{nil};     ///< Pointer to the event monitor handler
};

/**
 * @brief Construct the macOS platform application.
 * @param configPath Path to the configuration file.
 * 
 * Creates a new macOS platform application instance, initializing the base
 * Application class and creating the platform-specific implementation.
 */
PlatformApplication::PlatformApplication(const std::string& configPath)
    : Application(configPath)
    , pImpl_(std::make_unique<Impl>(getSignalManager(), getWindowManager())) {
    DEBUG_LOG("Creating MacOS platform application");
}

// Required for unique_ptr with incomplete type
PlatformApplication::~PlatformApplication() = default;

/**
 * @brief Run the macOS application.
 * @return Exit code from the application.
 * 
 * Delegates to the implementation's run method to handle the
 * Cocoa run loop and event processing.
 */
[[nodiscard]] auto PlatformApplication::run() -> int { return pImpl_->run(); }

/**
 * @brief Quit the macOS application.
 * 
 * Delegates to the implementation's quit method to handle proper
 * Cocoa application termination and cleanup.
 */
auto PlatformApplication::quit() -> void { pImpl_->quit(); }

}  // namespace interview_cheater