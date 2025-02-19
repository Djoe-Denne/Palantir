#include "window/overlay_window.hpp"
#import <Cocoa/Cocoa.h>

#ifdef DEBUG
#define DebugLog(format, ...) NSLog(@"%s:%d - " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define DebugLog(format, ...)
#endif

@interface OverlayWindowDelegate : NSObject<NSWindowDelegate>
@property (nonatomic, assign) OverlayWindow* owner;
@end

@implementation OverlayWindowDelegate
- (BOOL)windowShouldClose:(NSWindow *)sender {
    DebugLog(@"Window close requested");
    if (self.owner) {
        self.owner->setRunning(false);
    }
    return YES;
}

- (void)windowDidResize:(NSNotification *)notification {
    DebugLog(@"Window resized to: %@", NSStringFromRect([((NSWindow *)notification.object) frame]));
}

- (void)windowDidMove:(NSNotification *)notification {
    DebugLog(@"Window moved to: %@", NSStringFromRect([((NSWindow *)notification.object) frame]));
}

- (void)windowDidBecomeKey:(NSNotification *)notification {
    DebugLog(@"Window became key");
}

- (void)windowDidResignKey:(NSNotification *)notification {
    DebugLog(@"Window resigned key");
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

class OverlayWindow::Impl {
public:
    Impl() : window(nil), delegate(nil) {
        DebugLog(@"Creating OverlayWindow implementation");
    }
    
    void create() {
        DebugLog(@"Beginning window creation");
        NSRect frame = NSMakeRect(100, 100, 400, 600);
        NSUInteger styleMask = NSWindowStyleMaskTitled |
                              NSWindowStyleMaskClosable |
                              NSWindowStyleMaskResizable |
                              NSWindowStyleMaskNonactivatingPanel;
        
        window = [[NSPanel alloc] initWithContentRect:frame
                                           styleMask:styleMask
                                             backing:NSBackingStoreBuffered
                                               defer:NO];
        
        DebugLog(@"Window created with frame: %@", NSStringFromRect(frame));
        
        [window setTitle:@"Interview Notes"];
        [window setReleasedWhenClosed:NO];
        
        // Configure window behavior
        [window setLevel:NSFloatingWindowLevel];
        [window setCollectionBehavior:NSWindowCollectionBehaviorCanJoinAllSpaces |
                                    NSWindowCollectionBehaviorFullScreenAuxiliary |
                                    NSWindowCollectionBehaviorStationary];
        [window setSharingType:NSWindowSharingNone];
        
        DebugLog(@"Window behavior configured");
        
        // Make it a non-activating panel
        [(NSPanel*)window setBecomesKeyOnlyIfNeeded:YES];
        [(NSPanel*)window setFloatingPanel:YES];
        [window setHidesOnDeactivate:NO];
        [window setMovableByWindowBackground:YES];
        [window setAcceptsMouseMovedEvents:YES];
        [window setBackgroundColor:[NSColor colorWithCalibratedWhite:1.0 alpha:0.95]];
        
        DebugLog(@"Window panel properties set");
        
        setupContentView();
        positionWindow();
        
        DebugLog(@"Window creation complete");
        [window orderOut:nil];
    }
    
    void setupContentView() {
        DebugLog(@"Setting up content view");
        
        // Create and configure scroll view
        NSScrollView* scrollView = [[NSScrollView alloc] initWithFrame:[[window contentView] bounds]];
        [scrollView setBorderType:NSNoBorder];
        [scrollView setHasVerticalScroller:YES];
        [scrollView setHasHorizontalScroller:NO];
        [scrollView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
        
        // Create and configure text view
        NSTextView* textView = [[NSTextView alloc] initWithFrame:[scrollView bounds]];
        configureTextView(textView);
        
        // Set up view hierarchy
        [scrollView setDocumentView:textView];
        [window setContentView:scrollView];
        
        // Set up delegate
        delegate = [[OverlayWindowDelegate alloc] init];
        delegate.owner = (__bridge OverlayWindow*)this;
        [window setDelegate:delegate];
        
        DebugLog(@"Content view setup complete");
    }
    
    void configureTextView(NSTextView* textView) {
        [textView setMinSize:NSMakeSize(0.0, 0.0)];
        [textView setMaxSize:NSMakeSize(FLT_MAX, FLT_MAX)];
        [textView setVerticallyResizable:YES];
        [textView setHorizontallyResizable:NO];
        [textView setAutoresizingMask:NSViewWidthSizable];
        [textView setTextContainerInset:NSMakeSize(10, 10)];
        [textView setEditable:NO];
        [textView setFont:[NSFont systemFontOfSize:13.0]];
        [textView setTextColor:[NSColor textColor]];
        
        NSString* content = @"# Interview Notes\n\n"
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
        [[textView textContainer] setWidthTracksTextView:YES];
    }
    
    void positionWindow() {
        NSScreen* screen = [NSScreen mainScreen];
        NSRect screenFrame = [screen visibleFrame];
        NSRect windowFrame = [window frame];
        windowFrame.origin.x = screenFrame.origin.x + screenFrame.size.width - windowFrame.size.width - 20;
        windowFrame.origin.y = screenFrame.origin.y + screenFrame.size.height - windowFrame.size.height - 20;
        [window setFrame:windowFrame display:NO];
        DebugLog(@"Window positioned at: %@", NSStringFromRect(windowFrame));
    }
    
    void show() {
        if ([window isVisible]) {
            DebugLog(@"Hiding window");
            [window orderOut:nil];
        } else {
            DebugLog(@"Showing window");
            [window orderFrontRegardless];
        }
    }
    
    void close() {
        DebugLog(@"Closing window");
        [window close];
    }
    
    void* getNativeHandle() const {
        return (__bridge void*)window;
    }
    
    ~Impl() {
        DebugLog(@"Destroying window implementation");
        if (window) {
            [window setDelegate:nil];
            [window close];
            [delegate release];
        }
    }
    
private:
    NSWindow* window;
    OverlayWindowDelegate* delegate;
};

OverlayWindow::OverlayWindow() : pImpl(std::make_unique<Impl>()), running(false) {
    DebugLog(@"Creating OverlayWindow");
}

OverlayWindow::~OverlayWindow() = default;

void OverlayWindow::create() {
    DebugLog(@"Creating window");
    pImpl->create();
    running = true;
}

void OverlayWindow::show() {
    pImpl->show();
}

void OverlayWindow::update() {
    // No update needed
}

void OverlayWindow::close() {
    DebugLog(@"Closing window and setting running to false");
    pImpl->close();
    running = false;
}

void* OverlayWindow::getNativeHandle() const {
    return pImpl->getNativeHandle();
}

bool OverlayWindow::isRunning() const {
    return running;
}

void OverlayWindow::setRunning(bool runningState) {
    DebugLog(@"Setting running state to: %d", runningState);
    running = runningState;
} 