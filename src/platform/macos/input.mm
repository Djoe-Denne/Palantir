#include "platform/macos/input.hpp"
#include "platform/macos/key_codes.hpp"
#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>

@interface GlobalEventMonitor : NSObject
@property (nonatomic, retain) id globalEventMonitor;
@property (nonatomic, retain) id localEventMonitor;
@property (nonatomic) BOOL keyPressed;
@property (nonatomic) BOOL modifierActive;
@end

@implementation GlobalEventMonitor

- (instancetype)init {
    if (self = [super init]) {
        _keyPressed = NO;
        _modifierActive = NO;
        [self setupEventMonitors];
    }
    return self;
}

- (void)handleEvent:(NSEvent *)event {
    if (event.type == NSEventTypeKeyDown || event.type == NSEventTypeKeyUp) {
        if (event.keyCode == KeyCodes::KEY_SLASH) {
            self.keyPressed = (event.type == NSEventTypeKeyDown);
        }
    } else if (event.type == NSEventTypeFlagsChanged) {
        self.modifierActive = (event.modifierFlags & NSEventModifierFlagCommand) != 0;
    }
}

- (void)setupEventMonitors {
    // Monitor for events when app is not in focus
    self.globalEventMonitor = [NSEvent addGlobalMonitorForEventsMatchingMask:(NSEventMaskKeyDown | NSEventMaskKeyUp | NSEventMaskFlagsChanged)
                                                                    handler:^(NSEvent *event) {
        [self handleEvent:event];
    }];
    
    // Monitor for events when app is in focus
    self.localEventMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:(NSEventMaskKeyDown | NSEventMaskKeyUp | NSEventMaskFlagsChanged)
                                                                  handler:^NSEvent *(NSEvent *event) {
        [self handleEvent:event];
        return event;
    }];
}

- (void)dealloc {
    if (self.globalEventMonitor) {
        [NSEvent removeMonitor:self.globalEventMonitor];
    }
    if (self.localEventMonitor) {
        [NSEvent removeMonitor:self.localEventMonitor];
    }
    [super dealloc];
}

@end

class Input::Impl {
public:
    Impl() : monitor([[GlobalEventMonitor alloc] init]) {}
    
    bool isKeyPressed() const {
        return [monitor keyPressed];
    }
    
    bool isModifierActive() const {
        return [monitor modifierActive];
    }
    
    void update() {} // No-op for macOS as we use event-based monitoring

private:
    GlobalEventMonitor* monitor;
};

Input::Input() : pImpl(std::make_unique<Impl>()) {}
Input::~Input() = default;

bool Input::isKeyPressed() const { return pImpl->isKeyPressed(); }
bool Input::isModifierActive() const { return pImpl->isModifierActive(); }
void Input::update() { pImpl->update(); } 