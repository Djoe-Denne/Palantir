#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "command/clear_screenshot_command.hpp"
#include <filesystem>
#include <regex>
#include <fstream>

namespace palantir::command::test {

class ClearScreenshotCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Clean up any existing screenshot directory
        if (!std::filesystem::exists("./screenshot")) {
            std::filesystem::create_directory("./screenshot");
            std::ofstream file("./screenshot/screenshot_test.txt");
            file << "Test content";
            file.close();
        }
    }

    void TearDown() override {
        // Clean up after tests
        if (std::filesystem::exists("./screenshot")) {
            std::filesystem::remove_all("./screenshot");
        }
    }
};

TEST_F(ClearScreenshotCommandTest, ExecuteClearScreenshotFile) {
    ClearScreenshotCommand cmd;
    cmd.execute();

    bool found_screenshot = std::filesystem::exists("./screenshot/screenshot_test.txt");
    EXPECT_FALSE(found_screenshot);
}

TEST_F(ClearScreenshotCommandTest, DoesNotUseDebounce) {
    ClearScreenshotCommand cmd;
    EXPECT_FALSE(cmd.useDebounce());
}

} // namespace palantir::command::test 