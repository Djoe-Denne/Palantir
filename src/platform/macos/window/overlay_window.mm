#include "window/overlay_window.hpp"
#import <Cocoa/Cocoa.h>

@interface OverlayWindowDelegate : NSObject<NSWindowDelegate>
@property (nonatomic, assign) OverlayWindow* owner;
@end

@implementation OverlayWindowDelegate
- (BOOL)windowShouldClose:(NSWindow *)sender {
    if (self.owner) {
        self.owner->setRunning(false);
    }
    return YES;
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
    Impl() : window(nil), delegate(nil) {}
    
    void create() {
        NSRect frame = NSMakeRect(0, 0, 800, 600);
        NSUInteger styleMask = NSWindowStyleMaskTitled |
                              NSWindowStyleMaskClosable |
                              NSWindowStyleMaskMiniaturizable |
                              NSWindowStyleMaskResizable;
        
        window = [[NSWindow alloc] initWithContentRect:frame
                                           styleMask:styleMask
                                             backing:NSBackingStoreBuffered
                                               defer:NO];
        
        [window setTitle:@"Interview Cheater"];
        [window setReleasedWhenClosed:NO];
        
        // Set window to float above others and exclude from screen sharing
        [window setLevel:NSFloatingWindowLevel];
        [window setCollectionBehavior:NSWindowCollectionBehaviorTransient];
        [window setSharingType:NSWindowSharingNone];  // This excludes the window from screen sharing
        
        // Create content view
        ContentView* contentView = [[ContentView alloc] initWithFrame:frame];
        [window setContentView:contentView];
        
        // Create scroll view for text
        NSScrollView* scrollView = [[NSScrollView alloc] initWithFrame:frame];
        [scrollView setBorderType:NSNoBorder];
        [scrollView setHasVerticalScroller:YES];
        [scrollView setHasHorizontalScroller:NO];
        [scrollView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
        
        // Create text view
        NSTextView* textView = [[NSTextView alloc] initWithFrame:frame];
        [textView setMinSize:NSMakeSize(0.0, 0.0)];
        [textView setMaxSize:NSMakeSize(FLT_MAX, FLT_MAX)];
        [textView setVerticallyResizable:YES];
        [textView setHorizontallyResizable:NO];
        [textView setAutoresizingMask:NSViewWidthSizable];
        [textView setTextContainerInset:NSMakeSize(10, 10)];
        [textView setEditable:NO];  // Make text view read-only
        
        // Set some sample content
        NSString* sampleContent = @"# Interview Notes\n\n"
                                @"## Key Points to Remember\n\n"
                                @"1. Project Experience:\n"
                                @"   - Led team of 5 developers\n"
                                @"   - Improved performance by 40%\n"
                                @"   - Implemented CI/CD pipeline\n\n"
                                @"2. Technical Skills:\n"
                                @"   - C++17 features\n"
                                @"   - Design patterns\n"
                                @"   - System architecture\n\n"
                                @"3. Achievements:\n"
                                @"   - Reduced build time by 50%\n"
                                @"   - Zero production incidents\n"
                                @"   - Mentored 3 junior developers\n\n"
                                @"## Questions to Ask:\n\n"
                                @"1. Team structure and workflow\n"
                                @"2. Technical challenges\n"
                                @"3. Career growth opportunities\n\n"
                                @"Press Ctrl+F1 to toggle window visibility\n"
                                @"This window is hidden from screen sharing!";
        
        // Apply some basic styling
        NSFont* font = [NSFont systemFontOfSize:14.0];
        NSDictionary* attributes = @{
            NSFontAttributeName: font,
            NSForegroundColorAttributeName: [NSColor textColor]
        };
        
        NSAttributedString* attributedContent = [[NSAttributedString alloc] 
            initWithString:sampleContent
            attributes:attributes];
        
        [[textView textStorage] setAttributedString:attributedContent];
        
        // Configure text container
        NSTextContainer* container = [textView textContainer];
        [container setWidthTracksTextView:YES];
        
        // Set up scroll view
        [scrollView setDocumentView:textView];
        [contentView addSubview:scrollView];
        
        // Set up constraints
        scrollView.translatesAutoresizingMaskIntoConstraints = NO;
        [NSLayoutConstraint activateConstraints:@[
            [scrollView.topAnchor constraintEqualToAnchor:contentView.topAnchor],
            [scrollView.leadingAnchor constraintEqualToAnchor:contentView.leadingAnchor],
            [scrollView.trailingAnchor constraintEqualToAnchor:contentView.trailingAnchor],
            [scrollView.bottomAnchor constraintEqualToAnchor:contentView.bottomAnchor]
        ]];
        
        delegate = [[OverlayWindowDelegate alloc] init];
        delegate.owner = (__bridge OverlayWindow*)this;
        [window setDelegate:delegate];
        
        // Center window
        [window center];
        
        // Initially hide the window
        [window orderOut:nil];
    }
    
    void show() {
        if ([window isVisible]) {
            [window orderOut:nil];
        } else {
            [window makeKeyAndOrderFront:nil];
        }
    }
    
    void update() {
        // Process window events if needed
    }
    
    void close() {
        [window close];
    }
    
    void* getNativeHandle() const {
        return (__bridge void*)window;
    }
    
    ~Impl() {
        if (window) {
            [window setDelegate:nil];
            [window close];
        }
    }
    
private:
    NSWindow* window;
    OverlayWindowDelegate* delegate;
};

OverlayWindow::OverlayWindow() : pImpl(std::make_unique<Impl>()), running(false) {}
OverlayWindow::~OverlayWindow() = default;

void OverlayWindow::create() {
    pImpl->create();
    running = true;
}

void OverlayWindow::show() {
    pImpl->show();
}

void OverlayWindow::update() {
    pImpl->update();
}

void OverlayWindow::close() {
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
    running = runningState;
} 