#include "signal/signal_manager.hpp"

#import <Cocoa/Cocoa.h>
#include <vector>
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

namespace interview_cheater::signal {
    class SignalManager::Impl;
};

/**
 * @brief Signal checking class for macOS event monitoring.
 *
 * This Objective-C class handles the event monitoring for keyboard inputs
 * on macOS. It sets up both global and local monitors to capture keyboard
 * events and trigger signal checks. The class serves as a bridge between
 * the Cocoa event system and our C++ signal manager.
 */
@interface SignalChecker : NSObject

/** Pointer to the implementation that owns this checker */
@property(nonatomic, assign) interview_cheater::signal::SignalManager::Impl* pImpl_;

/** Monitor for global keyboard events (when app is not active) */
@property(nonatomic, strong) id globalMonitor;

/** Monitor for local keyboard events (when app is active) */
@property(nonatomic, strong) id localMonitor;

/**
 * @brief Initialize the signal checker with a signal manager implementation
 */
- (instancetype)initWithSignalManagerImpl:(interview_cheater::signal::SignalManager::Impl*)impl;

/**
 * @brief Stop checking events
 *
 * This method stops the event monitoring and cleans up the event monitors.
 */
- (void)stopChecking;

@end

namespace interview_cheater::signal {

class SignalManager::Impl {
public:
    explicit Impl(SignalManager* parent);
    ~Impl();

    // Delete copy and move operations
    Impl(const Impl&) = delete;
    auto operator=(const Impl&) -> Impl& = delete;
    Impl(Impl&&) = delete;
    auto operator=(Impl&&) -> Impl& = delete;

    auto addSignal(std::unique_ptr<ISignal> signal) -> void;
    auto startSignals() -> void;
    auto stopSignals() -> void;
    auto checkSignals(const std::any& event) -> void;

private:
    SignalChecker* signalChecker_{nullptr};  ///< The Objective-C event monitor instance
    SignalManager* parent_;                  ///< Pointer to the owning SignalManager
    std::vector<std::unique_ptr<ISignal>> signals_;  ///< Collection of managed signals
};

} // namespace interview_cheater::signal

@implementation SignalChecker

/**
 * @brief Initialize the signal checker and set up event monitors
 * @param impl Pointer to the C++ signal manager implementation
 * @return Initialized instance or nil on failure
 *
 * This method initializes the signal checker and immediately sets up
 * the event monitors to start capturing keyboard events.
 */
- (instancetype)initWithSignalManagerImpl:(interview_cheater::signal::SignalManager::Impl*)impl {
    if ((self = [super init]) != nil) {
        DEBUG_LOG("Initializing SignalChecker");
        self.pImpl_ = impl;
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
                                                   DEBUG_LOG("Global event received");
                                                   [self handleKeyEvent:event];
                                               }];

    // Local monitor for when app is active
    // This will receive events regardless of accessibility permissions
    self.localMonitor =
        [NSEvent addLocalMonitorForEventsMatchingMask:eventMask
                                              handler:^NSEvent*(NSEvent* event) {
                                                  DEBUG_LOG("Local event received");
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
        self.pImpl_->checkSignals(event);
    });
}

/**
 * @brief Stop checking events
 *
 * This method stops the event monitoring and cleans up the event monitors.
 */
- (void)stopChecking {
    DEBUG_LOG("Stopping event monitors");
    if (self.globalMonitor != nil) {
        [NSEvent removeMonitor:self.globalMonitor];
        self.globalMonitor = nil;
    }
    if (self.localMonitor != nil) {
        [NSEvent removeMonitor:self.localMonitor];
        self.localMonitor = nil;
    }
}

/**
 * @brief Clean up event monitors when the instance is deallocated
 *
 * This method ensures proper cleanup of the event monitors to prevent
 * memory leaks and invalid callbacks after the instance is destroyed.
 */
- (void)dealloc {
    DEBUG_LOG("Cleaning up SignalChecker");
    [self stopChecking];
}

@end

namespace interview_cheater::signal {
    
Impl::Impl(SignalManager* parent) : parent_(parent) {
    signalChecker_ = [[SignalChecker alloc] initWithSignalManagerImpl:this];
}

Impl::~Impl() {
    if (signalChecker_ != nullptr) {
        [signalChecker_ stopChecking];
        signalChecker_ = nil;  // ARC will handle the release
    }
}

auto Impl::addSignal(std::unique_ptr<ISignal> signal) -> void {
    signals_.push_back(std::move(signal));
}

auto Impl::startSignals() -> void {
    for (const auto& signal : signals_) {
        signal->start();
    }
}

auto Impl::stopSignals() -> void {
    for (const auto& signal : signals_) {
        signal->stop();
    }
}

auto Impl::checkSignals(const std::any& event) -> void {
    for (const auto& signal : signals_) {
        signal->check(event);
    }
}

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
    pImpl_->addSignal(std::move(signal));
}

/**
 * @brief Start monitoring all registered signals
 *
 * Calls the start() method on all registered signals.
 * The event monitoring system is already active from construction.
 */
auto SignalManager::startSignals() -> void {
    DEBUG_LOG("Starting signals");
    pImpl_->startSignals();
}

/**
 * @brief Stop monitoring all registered signals
 *
 * Calls the stop() method on all registered signals.
 * The event monitoring system remains active.
 */
auto SignalManager::stopSignals() -> void {
    DEBUG_LOG("Stopping signals");
    pImpl_->stopSignals();
}

/**
 * @brief Check all registered signals
 *
 * This method is called by the SignalChecker when keyboard events
 * are received. It triggers the check() method on all registered signals.
 * The method is called on the main thread for thread safety.
 */
auto SignalManager::checkSignals(const std::any& event) -> void {
    pImpl_->checkSignals(event);
}

auto SignalManager::getInstance() -> SignalManager& {
    static SignalManager instance;
    return instance;
}

}  // namespace interview_cheater::signal