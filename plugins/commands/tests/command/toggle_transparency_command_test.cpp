#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "command/toggle_transparency_command.hpp"
#include "mock/mock_application.hpp"
#include "mock/window/mock_window_manager.hpp"
#include "mock/window/mock_window.hpp"

using namespace palantir;
using namespace palantir::command;
using namespace palantir::test;
using namespace testing;

class ToggleTransparencyCommandTest : public Test {
protected:
    void SetUp() override {
        mockWindow = std::make_shared<MockWindow>();
        mockWindowManager = std::make_shared<MockWindowManager>();
        mockApp = std::make_shared<MockApplication>("");

        // Set up default behaviors
        EXPECT_CALL(*mockApp, getWindowManager())
            .WillRepeatedly(ReturnRef(palantir::window::WindowManager::getInstance()));
        
        palantir::Application::setInstance(mockApp);
        palantir::window::WindowManager::setInstance(mockWindowManager);
    }

    void TearDown() override {
        palantir::Application::setInstance(nullptr);
        palantir::window::WindowManager::setInstance(nullptr);
        mockApp.reset();
        mockWindow.reset();
        mockWindowManager.reset();
    }

    std::shared_ptr<MockWindow> mockWindow;
    std::shared_ptr<MockWindowManager> mockWindowManager;
    std::shared_ptr<MockApplication> mockApp;
};

TEST_F(ToggleTransparencyCommandTest, ExecuteTogglesTransparencyWhenWindowExists) {
    // Arrange
    EXPECT_CALL(*mockWindowManager, getMainWindow())
        .Times(2)
        .WillRepeatedly(Return(mockWindow));

    EXPECT_CALL(*mockWindow, getNativeHandle())
        .Times(1)
        .WillOnce(Return(0)); // TODO(@Djoe-Denne) create a mockable wrapper for window update function

    EXPECT_CALL(*mockWindow, setTransparency(100))
        .Times(1);

    ToggleTransparencyCommand command;

    // Act
    command.execute();
}

TEST_F(ToggleTransparencyCommandTest, ExecuteThrowsWhenNoWindow) {
    // Arrange
    EXPECT_CALL(*mockWindowManager, getMainWindow())
        .WillOnce(Return(nullptr));

    ToggleTransparencyCommand command;

    // Act & Assert
    EXPECT_THROW(command.execute(), std::runtime_error);
}

TEST_F(ToggleTransparencyCommandTest, UseDebounceReturnsTrue) {
    // Arrange
    ToggleTransparencyCommand command;

    // Act & Assert
    EXPECT_TRUE(command.useDebounce());
} 