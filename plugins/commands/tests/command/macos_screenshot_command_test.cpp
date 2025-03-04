#ifdef __APPLE__

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "command/window_screenshot_command.hpp"
#include <filesystem>
#include <ApplicationServices/ApplicationServices.h>

namespace palantir::command::test {

class MacOSScreenshotCommandTest : public ::testing::Test {
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
};

TEST_F(MacOSScreenshotCommandTest, CapturesActiveWindow) {
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

    EXPECT_TRUE(found_screenshot);
    // A valid screenshot should be larger than 1KB
    EXPECT_GT(fileSize, 1024);
}

TEST_F(MacOSScreenshotCommandTest, HandlesNoActiveWindow) {
    // Minimize all windows (this is a simplified test as we can't easily manipulate windows in unit tests)
    WindowScreenshotCommand cmd;
    cmd.execute();

    // Should still create directory
    EXPECT_TRUE(std::filesystem::exists("./screenshot"));
}

} // namespace palantir::command::test

#endif // __APPLE__ 