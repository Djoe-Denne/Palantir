#include "platform/macos/macos_application.hpp"
#include "platform/macos/key_codes.hpp"
#include "utils/logger.hpp"
#import <Cocoa/Cocoa.h>

@interface SignalChecker : NSObject
@property (nonatomic, assign) SignalManager* signalManager;
@property (nonatomic, strong) id globalMonitor;
@property (nonatomic, strong) id localMonitor;
- (instancetype)initWithSignalManager:(SignalManager*)signalManager;
@end

@implementation SignalChecker

- (instancetype)initWithSignalManager:(SignalManager*)signalManager {
    if (self = [super init]) {
        DEBUG_LOG("Initializing SignalChecker");
        self.signalManager = signalManager;
        [self setupMonitors];
    }
    return self;
}

- (void)setupMonitors {
    DEBUG_LOG("Setting up event monitors");
    NSEventMask eventMask = NSEventMaskKeyDown | NSEventMaskFlagsChanged;
    
    // Global monitor for when app is not active
    self.globalMonitor = [NSEvent addGlobalMonitorForEventsMatchingMask:eventMask handler:^(NSEvent *event) {
        DEBUG_LOG("Global event received: type=%lu, keyCode=%d, modifiers=0x%lx", 
                (unsigned long)event.type, (int)event.keyCode, (unsigned long)event.modifierFlags);
        [self handleKeyEvent:event];
    }];
    
    // Local monitor for when app is active
    self.localMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:eventMask handler:^NSEvent *(NSEvent *event) {
        DEBUG_LOG("Local event received: type=%lu, keyCode=%d, modifiers=0x%lx", 
                (unsigned long)event.type, (int)event.keyCode, (unsigned long)event.modifierFlags);
        [self handleKeyEvent:event];
        return event;
    }];
    
    if (!self.globalMonitor || !self.localMonitor) {
        DEBUG_LOG("Warning: Failed to create one or more event monitors");
    }
}

- (void)handleKeyEvent:(NSEvent *)event {
    // Check for Command + / combination
    if (event.type == NSEventTypeKeyDown &&
        event.keyCode == KeyCodes::KEY_SLASH &&
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
    if (self.globalMonitor) {
        [NSEvent removeMonitor:self.globalMonitor];
        DEBUG_LOG("Global monitor removed");
    }
    if (self.localMonitor) {
        [NSEvent removeMonitor:self.localMonitor];
        DEBUG_LOG("Local monitor removed");
    }
    [super dealloc];
}

@end

class MacOSApplication::Impl {
public:
    Impl(SignalManager& signalManager) : signalManager_(signalManager), signalChecker(nil) {
        DEBUG_LOG("Initializing MacOSApplication");
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyAccessory];
        
        // Request accessibility permissions if needed
        NSDictionary *options = @{(id)kAXTrustedCheckOptionPrompt: @YES};
        BOOL accessibilityEnabled = AXIsProcessTrustedWithOptions((CFDictionaryRef)options);
        
        DEBUG_LOG("Accessibility status: %@", accessibilityEnabled ? @"Enabled" : @"Disabled");
        
        if (!accessibilityEnabled) {
            NSLog(@"Please grant accessibility permissions in System Preferences > Security & Privacy > Privacy > Accessibility");
        }
        
        signalChecker = [[SignalChecker alloc] initWithSignalManager:&signalManager];
        DEBUG_LOG("Application initialization complete");
    }
    
    int run() {
        DEBUG_LOG("Starting application run loop");
        [NSApp run];
        return 0;
    }
    
    void quit() {
        DEBUG_LOG("Application quitting");
        [signalChecker release];
        signalChecker = nil;
        [NSApp terminate:nil];
    }
    
    ~Impl() {
        DEBUG_LOG("Application being destroyed");
        [signalChecker release];
        signalChecker = nil;
    }
    
private:
    SignalManager& signalManager_;
    SignalChecker* signalChecker;
};

MacOSApplication::MacOSApplication(SignalManager& signalManager) 
    : pImpl(std::make_unique<Impl>(signalManager)) {}

MacOSApplication::~MacOSApplication() = default;

int MacOSApplication::run() {
    return pImpl->run();
}

void MacOSApplication::quit() {
    pImpl->quit();
} 