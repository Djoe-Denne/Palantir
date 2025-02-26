#include "window/overlay_window.hpp"
#import <Cocoa/Cocoa.h>
#import "utils/logger.hpp"

namespace {
constexpr int K_WINDOW_INITIAL_X = 100;
constexpr int K_WINDOW_INITIAL_Y = 100;
constexpr int K_WINDOW_INITIAL_WIDTH = 400;
constexpr int K_WINDOW_INITIAL_HEIGHT = 600;
constexpr int K_WINDOW_MARGIN = 20;
constexpr int K_TEXT_CONTAINER_INSET = 10;
constexpr double K_WINDOW_ALPHA = 0.95;
constexpr double K_WINDOW_WHITE = 1.0;
constexpr double K_FONT_SIZE = 14.0;
}  // namespace

@interface OverlayWindowDelegate : NSObject <NSWindowDelegate>
@property(nonatomic, assign) interview_cheater::window::OverlayWindow* overlayWindow;
@end

@implementation OverlayWindowDelegate
- (BOOL)windowShouldClose:(NSWindow*)sender {
    DEBUG_LOG("Window close requested");
    if (self.overlayWindow != nil) {
        self.overlayWindow->setRunning(false);
    }
    return YES;
}

- (void)windowDidResize:(NSNotification*)notification {
    DEBUG_LOG("Window resized to: %s", [NSStringFromRect([((NSWindow*)notification.object) frame]) UTF8String]);
}

- (void)windowDidMove:(NSNotification*)notification {
    DEBUG_LOG("Window moved to: %s", [NSStringFromRect([((NSWindow*)notification.object) frame]) UTF8String]);
}

- (void)windowDidBecomeKey:(NSNotification*)notification {
    DEBUG_LOG("Window became key");
}

- (void)windowDidResignKey:(NSNotification*)notification {
    DEBUG_LOG("Window resigned key");
}
@end

@interface ContentView : NSView
@end

@implementation ContentView

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    [[NSColor windowBackgroundColor] set];
    NSRectFill(dirtyRect);
}

@end

namespace interview_cheater::window {

class OverlayWindow::Impl {
   public:
    Impl() = delete;
    explicit Impl(OverlayWindow* parent) : parent_(parent) {}
    Impl(const Impl&) = delete;
    Impl(Impl&&) = delete;
    auto operator=(const Impl&) -> Impl& = delete;
    auto operator=(Impl&&) -> Impl& = delete;

    auto create() -> void {
        DEBUG_LOG("Beginning window creation");
        NSRect frame =
            NSMakeRect(K_WINDOW_INITIAL_X, K_WINDOW_INITIAL_Y, K_WINDOW_INITIAL_WIDTH, K_WINDOW_INITIAL_HEIGHT);
        NSUInteger styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable |
                               NSWindowStyleMaskNonactivatingPanel;

        window_ = [[NSPanel alloc] initWithContentRect:frame
                                             styleMask:styleMask
                                               backing:NSBackingStoreBuffered
                                                 defer:NO];

        DEBUG_LOG("Window created with frame: %@", NSStringFromRect(frame));

        [window_ setTitle:@"Interview Notes"];
        [window_ setReleasedWhenClosed:NO];

        // Configure window behavior for stealth mode
        [window_ setLevel:NSFloatingWindowLevel];
        [window_ setCollectionBehavior:NSWindowCollectionBehaviorCanJoinAllSpaces |
                                       NSWindowCollectionBehaviorFullScreenAuxiliary |
                                       NSWindowCollectionBehaviorStationary];
        // TODO(@OopsOverflow): Implement this
        //                               NSWindowCollectionBehaviorTransient];
        [window_ setSharingType:NSWindowSharingNone];

        DEBUG_LOG("Window behavior configured");

        // Configure panel for stealth mode while maintaining functionality
        [(NSPanel*)window_ setBecomesKeyOnlyIfNeeded:YES];
        [(NSPanel*)window_ setFloatingPanel:YES];
        [window_ setHidesOnDeactivate:NO];
        [window_ setMovableByWindowBackground:YES];
        [window_ setAcceptsMouseMovedEvents:YES];
        [window_ setBackgroundColor:[NSColor colorWithCalibratedWhite:K_WINDOW_WHITE alpha:K_WINDOW_ALPHA]];
        // TODO(@OopsOverflow): Implement this
        // [window_ setTitlebarAppearsTransparent:YES];

        DEBUG_LOG("Window panel properties set");

        setupContentView();
        positionWindow();

        DEBUG_LOG("Window creation complete");
        [window_ orderOut:nil];
    }

    auto setupContentView() -> void {
        DEBUG_LOG("Setting up content view");

        @autoreleasepool {
            // Create and configure scroll view
            NSScrollView* scrollView = [[NSScrollView alloc] initWithFrame:[window_ contentView].bounds];
            [scrollView setHasVerticalScroller:YES];
            [scrollView setHasHorizontalScroller:NO];
            [scrollView setBorderType:NSNoBorder];
            [scrollView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];

            // Create and configure text view
            NSTextView* textView = [[NSTextView alloc] initWithFrame:scrollView.bounds];
            configureTextView(textView);

            // Set up view hierarchy
            [scrollView setDocumentView:textView];
            [window_ setContentView:scrollView];

            // Set up delegate
            delegate_ = [[OverlayWindowDelegate alloc] init];
            delegate_.overlayWindow = parent_;
            [window_ setDelegate:delegate_];
        }

        DEBUG_LOG("Content view setup complete");
    }

    auto configureTextView(NSTextView* const textView) -> void {
        [textView setMinSize:NSMakeSize(0.0, 0.0)];
        [textView setMaxSize:NSMakeSize(FLT_MAX, FLT_MAX)];
        [textView setVerticallyResizable:YES];
        [textView setHorizontallyResizable:NO];
        [textView setAutoresizingMask:NSViewWidthSizable];
        [textView setBackgroundColor:[NSColor clearColor]];
        [textView setTextColor:[NSColor whiteColor]];
        [textView setFont:[NSFont systemFontOfSize:K_FONT_SIZE]];
        [textView setTextContainerInset:NSMakeSize(K_TEXT_CONTAINER_INSET, K_TEXT_CONTAINER_INSET)];
        [textView setEditable:NO];
        [[textView textContainer] setContainerSize:NSMakeSize(FLT_MAX, FLT_MAX)];
        [[textView textContainer] setWidthTracksTextView:YES];

        NSString* const content = @"# Interview Notes\n\n"
                                  @"## Key Points\n\n"
                                  @"- Project Experience\n"
                                  @"- Technical Skills\n"
                                  @"- Problem Solving\n"
                                  @"- Team Collaboration\n\n"
                                  @"## Questions to Ask\n\n"
                                  @"1. Team structure and workflow\n"
                                  @"2. Technical challenges\n"
                                  @"3. Growth opportunities\n\n"
                                  @"Use Command + / to toggle this window\n"
                                  @"(Hidden from screen sharing)";

        [textView setString:content];
    }

    auto positionWindow() -> void {
        NSScreen* screen = [NSScreen mainScreen];
        NSRect screenFrame = screen.visibleFrame;
        NSRect windowFrame = window_.frame;
        windowFrame.origin.x = screenFrame.origin.x + screenFrame.size.width - windowFrame.size.width - K_WINDOW_MARGIN;
        windowFrame.origin.y =
            screenFrame.origin.y + screenFrame.size.height - windowFrame.size.height - K_WINDOW_MARGIN;
        [window_ setFrame:windowFrame display:NO];
        DEBUG_LOG("Window positioned at: %@", NSStringFromRect(windowFrame));
    }

    auto show() -> void {
        if ([window_ isVisible]) {
            DEBUG_LOG("Hiding window");
            [window_ orderOut:nil];
        } else {
            DEBUG_LOG("Showing window");
            [window_ orderFrontRegardless];
        }
    }

    auto close() -> void {
        DEBUG_LOG("Closing window");
        [window_ close];
    }

    // TODO(@OopsOverflow): Implement this
    auto setTransparency(int transparency) -> void { [window_ setAlphaValue:transparency / 100.0]; }

    // TODO(@OopsOverflow): Implement this
    auto toggleWindowAnonymity() -> void {
        /*if (window_ != nil) {
            NSWindowCollectionBehavior currentBehavior = [window_ collectionBehavior];
            if (currentBehavior & NSWindowCollectionBehaviorTransient) {
                // Switch to normal mode
                [window_ setCollectionBehavior:NSWindowCollectionBehaviorCanJoinAllSpaces |
                                             NSWindowCollectionBehaviorFullScreenAuxiliary |
                                             NSWindowCollectionBehaviorStationary];
                [window_ setTitlebarAppearsTransparent:NO];
                [(NSPanel*)window_ setBecomesKeyOnlyIfNeeded:NO];
            } else {
                // Switch to anonymous mode
                [window_ setCollectionBehavior:NSWindowCollectionBehaviorCanJoinAllSpaces |
                                             NSWindowCollectionBehaviorFullScreenAuxiliary |
                                             NSWindowCollectionBehaviorStationary |
                                             NSWindowCollectionBehaviorTransient];  // Makes it not appear in Mission
        Control [window_ setTitlebarAppearsTransparent:YES];
                [(NSPanel*)window_ setBecomesKeyOnlyIfNeeded:YES];
            }

            // Ensure window stays movable and resizable in both modes
            [window_ setMovableByWindowBackground:YES];
            [window_ setStyleMask:([window_ styleMask] | NSWindowStyleMaskResizable |
                                  NSWindowStyleMaskTitled | NSWindowStyleMaskClosable)];
        }*/
    }

    [[nodiscard]] auto getNativeHandle() const -> void* { return (__bridge void*)window_; }

    ~Impl() {
        DEBUG_LOG("Destroying window implementation");
        if (window_ != nil) {
            [window_ setDelegate:nil];
            [window_ close];
            window_ = nil;
        }
        if (delegate_ != nil) {
            delegate_ = nil;
        }
    }

   private:
    NSWindow* window_{nil};
    OverlayWindowDelegate* delegate_{nil};
    OverlayWindow* parent_;
};

OverlayWindow::OverlayWindow() : pImpl_(std::make_unique<Impl>(this)) {}
OverlayWindow::~OverlayWindow() = default;

auto OverlayWindow::create() -> void {
    DEBUG_LOG("Creating window");
    pImpl_->create();
    running_ = true;
}

auto OverlayWindow::show() -> void { pImpl_->show(); }

auto OverlayWindow::update() -> void {
    // No update needed
}

auto OverlayWindow::close() -> void {
    DEBUG_LOG("Closing window and setting running to false");
    pImpl_->close();
    running_ = false;
}

auto OverlayWindow::setTransparency(int transparency) -> void { pImpl_->setTransparency(transparency); }

auto OverlayWindow::toggleWindowAnonymity() -> void { pImpl_->toggleWindowAnonymity(); }

[[nodiscard]] auto OverlayWindow::getNativeHandle() const -> void* { return pImpl_->getNativeHandle(); }

[[nodiscard]] auto OverlayWindow::isRunning() const -> bool { return running_; }

auto OverlayWindow::setRunning(bool runningState) -> void {
    DEBUG_LOG("Setting running state to: %d", runningState);
    running_ = runningState;
}

}  // namespace interview_cheater::window