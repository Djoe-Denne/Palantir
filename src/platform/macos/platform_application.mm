#include "platform_application.hpp"
#import <Cocoa/Cocoa.h>
#include "input/key_codes.hpp"
#import "utils/logger.hpp"

@interface SignalChecker : NSObject
@property(nonatomic, assign) interview_cheater::signal::SignalManager* signalManager;
@property(nonatomic, strong) id globalMonitor;
@property(nonatomic, strong) id localMonitor;
- (instancetype)initWithSignalManager:(interview_cheater::signal::SignalManager*)signalManager;
@end

@implementation SignalChecker

- (instancetype)initWithSignalManager:(interview_cheater::signal::SignalManager*)signalManager {
    if (self = [super init]) {
        DEBUG_LOG("Initializing SignalChecker");
        self.signalManager = signalManager;
        [self setupEventMonitors];
    }
    return self;
}

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

- (void)handleKeyEvent:(NSEvent*)event {
    // Check for Command + / combination
    if (event.type == NSEventTypeKeyDown && event.keyCode == interview_cheater::input::KeyCodes::KEY_SLASH &&
        (event.modifierFlags & NSEventModifierFlagCommand)) {
        DEBUG_LOG("Hotkey combination detected (Command + /)");
        dispatch_async(dispatch_get_main_queue(), ^{
            DEBUG_LOG("Triggering signal check");
            self.signalManager->checkSignals();
        });
    }
}

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

class PlatformApplication::Impl {
   public:
    Impl(const Impl& other) = delete;
    auto operator=(const Impl& other) -> Impl& = delete;
    Impl(Impl&& other) noexcept = default;
    auto operator=(Impl&& other) noexcept -> Impl& = default;

    explicit Impl(signal::SignalManager& signalManager) : signalManager_(signalManager) {
        DEBUG_LOG("Initializing ");
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyAccessory];

        // Request accessibility permissions if needed
        NSDictionary* options = @{(id)kAXTrustedCheckOptionPrompt : @YES};
        bool accessibilityEnabled = AXIsProcessTrustedWithOptions((CFDictionaryRef)options);

        DEBUG_LOG("Accessibility status: %s", accessibilityEnabled ? "Enabled" : "Disabled");

        if (!accessibilityEnabled) {
            NSLog(@"Please grant accessibility permissions in System Preferences > "
                  @"Security & Privacy > Privacy > Accessibility");
        }

        signalChecker_ = [[SignalChecker alloc] initWithSignalManager:&signalManager];
        DEBUG_LOG("Application initialization complete");
    }

    [[nodiscard]] auto run() -> int {
        DEBUG_LOG("Starting application run loop");
        [NSApp run];
        return 0;
    }

    auto quit() -> void {
        DEBUG_LOG("Application quitting");
        if (signalChecker_ != nil) {
            signalChecker_ = nil;
        }
        [NSApp terminate:nil];
    }

    ~Impl() {
        DEBUG_LOG("Application being destroyed");
        if (signalChecker_ != nil) {
            signalChecker_ = nil;
        }
    }

   private:
    signal::SignalManager& signalManager_;
    SignalChecker* signalChecker_{nil};
};

PlatformApplication::PlatformApplication(signal::SignalManager& signalManager)
    : pImpl_(std::make_unique<Impl>(signalManager)) {}

PlatformApplication::~PlatformApplication() = default;

[[nodiscard]] auto PlatformApplication::run() -> int { return pImpl_->run(); }

auto PlatformApplication::quit() -> void { pImpl_->quit(); }

}  // namespace interview_cheater