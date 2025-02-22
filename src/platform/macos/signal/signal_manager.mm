#include "signal/signal_manager.hpp"

#import <Cocoa/Cocoa.h>
#include "signal/isignal.hpp"
#include "utils/logger.hpp"

/**
 * @file signal_manager.mm
 * @brief macOS implementation of the signal manager using NSEvent monitoring
 *
 * This file implements the SignalManager class for macOS using Objective-C++.
 * It uses the NSEvent monitoring system to capture global keyboard events,
 * even when the application is not in focus. The implementation follows the
 * bridge pattern between C++ and Objective-C.
 */

/**
 * @brief Signal checking class for macOS event monitoring.
 *
 * This Objective-C class handles the event monitoring for keyboard inputs
 * on macOS. It sets up both global and local monitors to capture keyboard
 * events and trigger signal checks. The class serves as a bridge between
 * the Cocoa event system and our C++ signal manager.
 */
@interface SignalChecker : NSObject

/** Pointer to the C++ signal manager that owns this checker */
@property(nonatomic, assign) interview_cheater::signal::SignalManager* signalManager;

/** Monitor for global keyboard events (when app is not active) */
@property(nonatomic, strong) id globalMonitor;

/** Monitor for local keyboard events (when app is active) */
@property(nonatomic, strong) id localMonitor;

/**
 * @brief Initialize the signal checker with a signal manager
 * @param signalManager Pointer to the owning signal manager instance
 * @return Initialized instance or nil on failure
 */
- (instancetype)initWithSignalManager:(interview_cheater::signal::SignalManager*)signalManager;

@end

@implementation SignalChecker

/**
 * @brief Initialize the signal checker and set up event monitors
 * @param signalManager Pointer to the C++ signal manager
 * @return Initialized instance or nil on failure
 *
 * This method initializes the signal checker and immediately sets up
 * the event monitors to start capturing keyboard events.
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
 * @brief Set up the keyboard event monitors
 *
 * This method creates two types of event monitors:
 * 1. Global monitor: Captures events when app is not active
 * 2. Local monitor: Captures events when app is active
 *
 * Both monitors watch for:
 * - Key down events (NSEventMaskKeyDown)
 * - Key up events (NSEventMaskKeyUp)
 * - Modifier key changes (NSEventMaskFlagsChanged)
 *
 * Note: Global event monitoring requires accessibility permissions
 * to be granted by the user in System Preferences.
 */
- (void)setupEventMonitors {
    DEBUG_LOG("Setting up event monitors");
    // Monitor key down/up and modifier key events
    NSEventMask eventMask = NSEventMaskKeyDown | NSEventMaskKeyUp | NSEventMaskFlagsChanged;

    // Global monitor for when app is not active
    // Note: This won't receive events if accessibility permissions aren't granted
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
    // This will receive events regardless of accessibility permissions
    self.localMonitor =
        [NSEvent addLocalMonitorForEventsMatchingMask:eventMask
                                              handler:^NSEvent*(NSEvent* event) {
                                                  DEBUG_LOG("Local event received: type=%lu, "
                                                            "keyCode=%d, modifiers=0x%lx",
                                                            (unsigned long)event.type, (int)event.keyCode,
                                                            (unsigned long)event.modifierFlags);
                                                  [self handleKeyEvent:event];
                                                  return event;  // Return event to allow it to propagate
                                              }];

    if (self.globalMonitor == nil || self.localMonitor == nil) {
        DEBUG_LOG("Warning: Failed to create one or more event monitors");
    }
}

/**
 * @brief Handle a keyboard event by triggering signal checks
 * @param event The NSEvent containing keyboard event information
 *
 * This method is called for both global and local keyboard events.
 * It dispatches the signal check to the main queue to ensure thread safety,
 * as event monitors may be called from different threads.
 */
- (void)handleKeyEvent:(NSEvent*)event {
    // Dispatch signal check to main queue for thread safety
    dispatch_async(dispatch_get_main_queue(), ^{
        DEBUG_LOG("Triggering signal check");
        self.signalManager->checkSignals(event);
    });
}

/**
 * @brief Clean up event monitors when the instance is deallocated
 *
 * This method ensures proper cleanup of the event monitors to prevent
 * memory leaks and invalid callbacks after the instance is destroyed.
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

namespace interview_cheater::signal {

/**
 * @brief Private implementation of SignalManager
 *
 * This class implements the PIMPL idiom to hide the Objective-C++
 * implementation details from the pure C++ interface. It manages
 * the lifecycle of the SignalChecker instance.
 */
class SignalManager::Impl {
   public:
    /**
     * @brief Construct the implementation
     * @param parent Pointer to the owning SignalManager instance
     *
     * Creates and initializes the SignalChecker instance that will
     * handle the actual event monitoring.
     */
    explicit Impl(SignalManager* parent) : parent_(parent) {
        signalChecker_ = [[SignalChecker alloc] initWithSignalManager:parent];
    }

    /**
     * @brief Destroy the implementation
     *
     * Ensures proper cleanup of the SignalChecker instance.
     * The nil assignment triggers Objective-C ARC to release the object.
     */
    ~Impl() {
        if (signalChecker_ != nil) {
            signalChecker_ = nil;
        }
    }

   private:
    SignalChecker* signalChecker_{nil};  ///< The Objective-C event monitor instance
    SignalManager* parent_;              ///< Pointer to the owning SignalManager
};

/**
 * @brief Construct the SignalManager
 *
 * Creates the private implementation which sets up the event monitoring system.
 */
SignalManager::SignalManager() : pImpl_(std::make_unique<Impl>(this)) { DEBUG_LOG("Initializing SignalManager"); }

// Default destructor is sufficient as pImpl_ is a unique_ptr
SignalManager::~SignalManager() = default;

/**
 * @brief Add a signal to be monitored
 * @param signal Unique pointer to the signal instance
 *
 * Adds a signal to the list of signals that will be checked
 * when keyboard events are received.
 */
auto SignalManager::addSignal(std::unique_ptr<ISignal> signal) -> void {
    DEBUG_LOG("Adding signal to manager");
    signals_.push_back(std::move(signal));
}

/**
 * @brief Start monitoring all registered signals
 *
 * Calls the start() method on all registered signals.
 * The event monitoring system is already active from construction.
 */
auto SignalManager::startSignals() -> void {
    DEBUG_LOG("Starting signals");
    for (const auto& signal : signals_) {
        signal->start();
    }
}

/**
 * @brief Stop monitoring all registered signals
 *
 * Calls the stop() method on all registered signals.
 * The event monitoring system remains active.
 */
auto SignalManager::stopSignals() -> void {
    DEBUG_LOG("Stopping signals");
    for (const auto& signal : signals_) {
        signal->stop();
    }
}

/**
 * @brief Check all registered signals
 *
 * This method is called by the SignalChecker when keyboard events
 * are received. It triggers the check() method on all registered signals.
 * The method is called on the main thread for thread safety.
 */
auto SignalManager::checkSignals(const std::any& event) -> void {
    for (const auto& signal : signals_) {
        signal->check(event);
    }
}

}  // namespace interview_cheater::signal