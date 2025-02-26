#include "command/window_screenshot_command.hpp"
#include <ApplicationServices/ApplicationServices.h>
#include <CoreGraphics/CoreGraphics.h>
#include <iostream>

namespace interview_cheater::command {

void WindowScreenshotCommand::captureScreenshot() {
    CGWindowID windowID = kCGNullWindowID;
    CFArrayRef windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
    CFIndex count = CFArrayGetCount(windowList);

    for (CFIndex i = 0; i < count; i++) {
        CFDictionaryRef windowInfo = (CFDictionaryRef)CFArrayGetValueAtIndex(windowList, i);
        CFNumberRef windowIDRef = (CFNumberRef)CFDictionaryGetValue(windowInfo, kCGWindowNumber);
        CFNumberGetValue(windowIDRef, kCFNumberIntType, &windowID);
        break; // First window (focused)
    }
    CFRelease(windowList);

    if (windowID == kCGNullWindowID) return;

    CGImageRef screenshot = CGWindowListCreateImage(
        CGRectNull, kCGWindowListOptionIncludingWindow, windowID, kCGWindowImageDefault);
    
    if (!screenshot) return;

    CFStringRef filePath = CFStringCreateWithCString(nullptr, generateFilePath().c_str(), kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(nullptr, filePath, kCFURLPOSIXPathStyle, false);

    CGImageDestinationRef dest = CGImageDestinationCreateWithURL(url, kUTTypePNG, 1, nullptr);
    CGImageDestinationAddImage(dest, screenshot, nullptr);
    CGImageDestinationFinalize(dest);

    CFRelease(dest);
    CFRelease(url);
    CFRelease(filePath);
    CGImageRelease(screenshot);
}

} // namespace interview_cheater::command

