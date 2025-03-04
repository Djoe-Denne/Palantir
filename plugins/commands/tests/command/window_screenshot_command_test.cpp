#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "command/window_screenshot_command.hpp"
#include <filesystem>
#include <regex>

namespace palantir::command::test {

class WindowScreenshotCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Clean up any existing screenshot directory
        if (std::filesystem::exists("./screenshot")) {
            std::filesystem::remove_all("./screenshot");
        }
    }

    void TearDown() override {
        // Clean up after tests
        if (std::filesystem::exists("./screenshot")) {
            std::filesystem::remove_all("./screenshot");
        }
    }

    // Helper function to validate screenshot file name format
    bool isValidScreenshotFileName(const std::string& filename) {
        std::regex pattern(R"(screenshot_\d{4}-\d{2}-\d{2}_\d{2}-\d{2}-\d{2}\.png)");
        return std::regex_match(filename, pattern);
    }
};

TEST_F(WindowScreenshotCommandTest, CreatesScreenshotDirectory) {
    WindowScreenshotCommand cmd;
    EXPECT_TRUE(std::filesystem::exists("./screenshot"));
    EXPECT_TRUE(std::filesystem::is_directory("./screenshot"));
}

TEST_F(WindowScreenshotCommandTest, ExecuteCreatesScreenshotFile) {
    WindowScreenshotCommand cmd;
    cmd.execute();

    // Check if at least one file was created
    bool found_screenshot = false;
    for (const auto& entry : std::filesystem::directory_iterator("./screenshot")) {
        if (isValidScreenshotFileName(entry.path().filename().string())) {
            found_screenshot = true;
            break;
        }
    }
    EXPECT_TRUE(found_screenshot);
}

TEST_F(WindowScreenshotCommandTest, DoesNotUseDebounce) {
    WindowScreenshotCommand cmd;
    EXPECT_FALSE(cmd.useDebounce());
}

} // namespace palantir::command::test 