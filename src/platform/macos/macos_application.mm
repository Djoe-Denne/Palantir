#include "platform/macos/macos_application.hpp"
#import <Cocoa/Cocoa.h>

@interface SignalChecker : NSObject
- (void)checkSignals:(NSTimer*)timer;
@end

@implementation SignalChecker {
    SignalManager* _signalManager;
}

- (instancetype)initWithSignalManager:(SignalManager*)signalManager {
    if (self = [super init]) {
        _signalManager = signalManager;
    }
    return self;
}

- (void)checkSignals:(NSTimer*)timer {
    _signalManager->checkSignals();
}

@end

class MacOSApplication::Impl {
public:
    Impl(SignalManager& signalManager) : signalManager_(signalManager), signalChecker(nil), signalTimer(nil) {
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        [NSApp activateIgnoringOtherApps:YES];
        
        // Set up signal checking timer
        signalChecker = [[SignalChecker alloc] initWithSignalManager:&signalManager];
        signalTimer = [NSTimer scheduledTimerWithTimeInterval:0.01  // Check every 10ms
                                                      target:signalChecker
                                                    selector:@selector(checkSignals:)
                                                    userInfo:nil
                                                     repeats:YES];
    }
    
    int run() {
        [NSApp run];
        return 0;
    }
    
    void quit() {
        if (signalTimer) {
            [signalTimer invalidate];
            signalTimer = nil;
        }
        signalChecker = nil;
        [NSApp terminate:nil];
    }
    
    ~Impl() {
        if (signalTimer) {
            [signalTimer invalidate];
        }
        signalChecker = nil;
    }
    
private:
    SignalManager& signalManager_;
    SignalChecker* signalChecker;
    NSTimer* signalTimer;
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