#include "platform_application.hpp"
#include "input/key_codes.hpp"
#import <Cocoa/Cocoa.h>

#ifdef DEBUG
#define DebugLog(format, ...) NSLog(@"%s:%d - " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define DebugLog(format, ...)
#endif

@interface SignalChecker : NSObject
@property (nonatomic, assign) SignalManager* signalManager;
@property (nonatomic, strong) id globalMonitor;
@property (nonatomic, strong) id localMonitor;
- (instancetype)initWithSignalManager:(SignalManager*)signalManager;
@end

@implementation SignalChecker

- (instancetype)initWithSignalManager:(SignalManager*)signalManager {
    if (self = [super init]) {
        DebugLog(@"Initializing SignalChecker");
        self.signalManager = signalManager;
        [self setupMonitors];
    }
    return self;
}

- (void)setupMonitors {
    DebugLog(@"Setting up event monitors");
    NSEventMask eventMask = NSEventMaskKeyDown | NSEventMaskFlagsChanged;
    
    // Global monitor for when app is not active
    self.globalMonitor = [NSEvent addGlobalMonitorForEventsMatchingMask:eventMask handler:^(NSEvent *event) {
        DebugLog(@"Global event received: type=%lu, keyCode=%d, modifiers=0x%lx", 
                (unsigned long)event.type, (int)event.keyCode, (unsigned long)event.modifierFlags);
        [self handleKeyEvent:event];
    }];
    
    // Local monitor for when app is active
    self.localMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:eventMask handler:^NSEvent *(NSEvent *event) {
        DebugLog(@"Local event received: type=%lu, keyCode=%d, modifiers=0x%lx", 
                (unsigned long)event.type, (int)event.keyCode, (unsigned long)event.modifierFlags);
        [self handleKeyEvent:event];
        return event;
    }];
    
    if (!self.globalMonitor || !self.localMonitor) {
        DebugLog(@"Warning: Failed to create one or more event monitors");
    }
}

- (void)handleKeyEvent:(NSEvent *)event {
    // Check for Command + / combination
    if (event.type == NSEventTypeKeyDown &&
        event.keyCode == KeyCodes::KEY_SLASH &&
        (event.modifierFlags & NSEventModifierFlagCommand)) {
        DebugLog(@"Hotkey combination detected (Command + /)");
        dispatch_async(dispatch_get_main_queue(), ^{
            DebugLog(@"Triggering signal check");
            self.signalManager->checkSignals();
        });
    }
}

- (void)dealloc {
    DebugLog(@"Cleaning up SignalChecker");
    if (self.globalMonitor) {
        [NSEvent removeMonitor:self.globalMonitor];
        DebugLog(@"Global monitor removed");
    }
    if (self.localMonitor) {
        [NSEvent removeMonitor:self.localMonitor];
        DebugLog(@"Local monitor removed");
    }
    [super dealloc];
}

@end

namespace interview_cheater {

class ::Impl {
public:
    Impl(signal::SignalManager& signalManager) : signalManager_(signalManager), signalChecker(nil) {
        DebugLog(@"Initializing ");
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyAccessory];
        
        // Request accessibility permissions if needed
        NSDictionary *options = @{(id)kAXTrustedCheckOptionPrompt: @YES};
        BOOL accessibilityEnabled = AXIsProcessTrustedWithOptions((CFDictionaryRef)options);
        
        DebugLog(@"Accessibility status: %@", accessibilityEnabled ? @"Enabled" : @"Disabled");
        
        if (!accessibilityEnabled) {
            NSLog(@"Please grant accessibility permissions in System Preferences > Security & Privacy > Privacy > Accessibility");
        }
        
        signalChecker = [[SignalChecker alloc] initWithSignalManager:&signalManager];
        DebugLog(@"Application initialization complete");
    }
    
    int run() {
        DebugLog(@"Starting application run loop");
        [NSApp run];
        return 0;
    }
    
    void quit() {
        DebugLog(@"Application quitting");
        [signalChecker release];
        signalChecker = nil;
        [NSApp terminate:nil];
    }
    
    ~Impl() {
        DebugLog(@"Application being destroyed");
        [signalChecker release];
        signalChecker = nil;
    }
    
private:
    signal::SignalManager& signalManager_;
    SignalChecker* signalChecker;
};

::(signal::SignalManager& signalManager) 
    : pImpl(std::make_unique<Impl>(signalManager)) {}

::~() = default;

int ::run() {
    return pImpl->run();
}

void ::quit() {
    pImpl->quit();
}

} // namespace interview_cheater