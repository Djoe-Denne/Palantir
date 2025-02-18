#include "macos/window/overlay_window.hpp"

OverlayWindow::OverlayWindow() : window(nil), running(false) {}

OverlayWindow::~OverlayWindow() {
    if (window) {
        [window close];
        [window release];
    }
}

void OverlayWindow::create() {
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
    [window setLevel:NSFloatingWindowLevel];
    [window setCollectionBehavior:NSWindowCollectionBehaviorCanJoinAllSpaces];
    [window setBackgroundColor:[NSColor clearColor]];
    [window setOpaque:NO];
    [window setHasShadow:NO];
    
    // Center the window
    [window center];
}

IWindow::NativeHandle OverlayWindow::getHandle() const {
    return window;
}

void OverlayWindow::show() {
    if (window) {
        [window makeKeyAndOrderFront:nil];
        running = true;
    }
}

void OverlayWindow::update() {
    if (window) {
        [[window contentView] setNeedsDisplay:YES];
    }
}

void OverlayWindow::close() {
    if (window) {
        [window close];
        running = false;
    }
}

bool OverlayWindow::isRunning() const {
    return running;
}

void OverlayWindow::setRunning(bool runningState) {
    running = runningState;
} 