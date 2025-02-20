#include "input/input.hpp"
#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>
#include "input/key_codes.hpp"

@interface GlobalEventMonitor : NSObject
@property(nonatomic, retain) id globalEventMonitor;
@property(nonatomic, retain) id localEventMonitor;
@property(nonatomic) BOOL keyPressed;
@property(nonatomic) BOOL modifierActive;
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

- (void)handleEvent:(NSEvent*)event {
    if (event.type == NSEventTypeKeyDown || event.type == NSEventTypeKeyUp) {
        if (event.keyCode == interview_cheater::input::KeyCodes::KEY_SLASH) {
            self.keyPressed = (event.type == NSEventTypeKeyDown);
        }
    } else if (event.type == NSEventTypeFlagsChanged) {
        self.modifierActive = (event.modifierFlags & NSEventModifierFlagCommand) != 0;
    }
}

- (void)setupEventMonitors {
    // Monitor for events when app is not in focus
    self.globalEventMonitor =
        [NSEvent addGlobalMonitorForEventsMatchingMask:(NSEventMaskKeyDown | NSEventMaskKeyUp | NSEventMaskFlagsChanged)
                                               handler:^(NSEvent* event) {
                                                   [self handleEvent:event];
                                               }];

    // Monitor for events when app is in focus
    self.localEventMonitor =
        [NSEvent addLocalMonitorForEventsMatchingMask:(NSEventMaskKeyDown | NSEventMaskKeyUp | NSEventMaskFlagsChanged)
                                              handler:^NSEvent*(NSEvent* event) {
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

namespace interview_cheater::input {

class Input::Impl {
   public:
    Impl() : monitor([[GlobalEventMonitor alloc] init]) {}

    auto isKeyPressed() const -> bool { return static_cast<BOOL>([monitor keyPressed]); }

    auto isModifierActive() const -> bool { return static_cast<BOOL>([monitor modifierActive]); }

    void update() {
        // No-op for macOS as we use event-based monitoring
    }

   private:
    GlobalEventMonitor* monitor;
};

}  // namespace interview_cheater::input

// Use the fully-qualified namespace for definitions
interview_cheater::input::Input::Input() : pImpl(std::make_unique<interview_cheater::input::Input::Impl>()) {}
interview_cheater::input::Input::~Input() = default;
auto interview_cheater::input::Input::isKeyPressed() const -> bool { return pImpl->isKeyPressed(); }
auto interview_cheater::input::Input::isModifierActive() const -> bool { return pImpl->isModifierActive(); }
void interview_cheater::input::Input::update() { pImpl->update(); }