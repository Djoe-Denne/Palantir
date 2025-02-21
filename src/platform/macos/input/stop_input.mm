#include "input/stop_input.hpp"
#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>
#include "input/key_codes.hpp"

@interface StopEventMonitor : NSObject
@property(nonatomic, retain) id globalEventMonitor;
@property(nonatomic, retain) id localEventMonitor;
@property(nonatomic) BOOL isKeyPressed;
@property(nonatomic) BOOL isModifierActive;
@end

@implementation StopEventMonitor

- (instancetype)init {
    if ((self = [super init]) != nil) {
        _isKeyPressed = NO;
        _isModifierActive = NO;
        [self setupEventMonitors];
    }
    return self;
}

- (void)handleEvent:(NSEvent*)event {
    if (event.type == NSEventTypeKeyDown || event.type == NSEventTypeKeyUp) {
        if (event.keyCode == interview_cheater::input::KeyCodes::KEY_SLASH) {
            self.isKeyPressed = (event.type == NSEventTypeKeyDown);
        }
    } else if (event.type == NSEventTypeFlagsChanged) {
        self.isModifierActive = (event.modifierFlags & interview_cheater::input::KeyCodes::COMMAND_MODIFIER) != 0;
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
    if (self.globalEventMonitor != nil) {
        [NSEvent removeMonitor:self.globalEventMonitor];
    }
    if (self.localEventMonitor != nil) {
        [NSEvent removeMonitor:self.localEventMonitor];
    }
}

@end

namespace interview_cheater::input {

class StopInput::Impl {
   public:
    Impl() = default;

    Impl(const Impl&) = delete;
    Impl(Impl&&) = delete;
    auto operator=(const Impl&) -> Impl& = delete;
    auto operator=(Impl&&) -> Impl& = delete;

    [[nodiscard]] auto isKeyPressed() const -> bool { return [monitor_ isKeyPressed] != NO; }

    [[nodiscard]] auto isModifierActive() const -> bool { return [monitor_ isModifierActive] != NO; }

    auto update() -> void {
        // No-op for macOS as we use event-based monitoring
    }

    ~Impl() {
        if (monitor_ != nil) {
            monitor_ = nil;
        }
    }

   private:
    StopEventMonitor* monitor_{[[StopEventMonitor alloc] init]};
};

}  // namespace interview_cheater::input

// Use the fully-qualified namespace for definitions
interview_cheater::input::StopInput::StopInput() : pImpl_(std::make_unique<Impl>()) {}
interview_cheater::input::StopInput::~StopInput() = default;
[[nodiscard]] auto interview_cheater::input::StopInput::isKeyPressed() const -> bool { return pImpl_->isKeyPressed(); }
[[nodiscard]] auto interview_cheater::input::StopInput::isModifierActive() const -> bool {
    return pImpl_->isModifierActive();
}
auto interview_cheater::input::StopInput::update() -> void { pImpl_->update(); } 