#ifdef _WIN32

#define UNICODE
#define _UNICODE
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "command/window_screenshot_command.hpp"
#include <windows.h>
#include <filesystem>

namespace palantir::command::test {

class WindowsScreenshotCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (std::filesystem::exists("./screenshot")) {
            std::filesystem::remove_all("./screenshot");
        }
    }

    void TearDown() override {
        if (std::filesystem::exists("./screenshot")) {
            std::filesystem::remove_all("./screenshot");
        }
    }

    // Helper to create a test window
    HWND createTestWindow() {
        const wchar_t* className = L"TestWindowClass";
        
        WNDCLASSEXW wc = {0};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.lpfnWndProc = DefWindowProcW;
        wc.hInstance = GetModuleHandleW(nullptr);
        wc.lpszClassName = className;
        
        if (!RegisterClassExW(&wc)) {
            return nullptr;
        }

        HWND hwnd = CreateWindowExW(
            0,
            className,
            L"Test Window",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            400, 300,
            nullptr,
            nullptr,
            GetModuleHandleW(nullptr),
            nullptr
        );

        if (hwnd) {
            ShowWindow(hwnd, SW_SHOW);
            UpdateWindow(hwnd);
            SetForegroundWindow(hwnd);
        }
        
        return hwnd;
    }
};

TEST_F(WindowsScreenshotCommandTest, CapturesActiveWindow) {
    HWND testWindow = createTestWindow();
    ASSERT_NE(testWindow, nullptr) << "Failed to create test window";

    // Give Windows some time to actually show the window
    Sleep(100);

    WindowScreenshotCommand cmd;
    cmd.execute();

    // Verify screenshot was created
    bool found_screenshot = false;
    std::uintmax_t fileSize = 0;
    for (const auto& entry : std::filesystem::directory_iterator("./screenshot")) {
        if (entry.path().extension() == ".png") {
            found_screenshot = true;
            fileSize = std::filesystem::file_size(entry.path());
            break;
        }
    }

    // Clean up test window
    if (testWindow) {
        DestroyWindow(testWindow);
        UnregisterClassW(L"TestWindowClass", GetModuleHandleW(nullptr));
    }

    EXPECT_TRUE(found_screenshot) << "Screenshot file was not created";
    EXPECT_GT(fileSize, 1024) << "Screenshot file is too small, might be empty";
}

TEST_F(WindowsScreenshotCommandTest, HandlesNoActiveWindow) {
    // Minimize all windows
    ShowWindow(GetForegroundWindow(), SW_MINIMIZE);
    
    WindowScreenshotCommand cmd;
    cmd.execute();

    // Should still create directory but might not create screenshot
    EXPECT_TRUE(std::filesystem::exists("./screenshot"));
}

} // namespace palantir::command::test

#endif // _WIN32 