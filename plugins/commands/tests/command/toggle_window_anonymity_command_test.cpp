#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "command/toggle_window_anonymity_command.hpp"
#include "mock/mock_application.hpp"
#include "mock/window/mock_window_manager.hpp"
#include "mock/window/mock_window.hpp"

using namespace palantir;
using namespace palantir::command;
using namespace palantir::test;
using namespace testing;

class ToggleWindowAnonymityCommandTest : public Test {
protected:
    void SetUp() override {
        mockWindow = std::make_shared<MockWindow>();
        mockWindowManager = std::make_shared<MockWindowManager>();
        mockApp = std::make_shared<MockApplication>("");

        // Set up default behaviors
        EXPECT_CALL(*mockApp, getWindowManager())
            .WillRepeatedly(Return(mockWindowManager));
            
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

TEST_F(ToggleWindowAnonymityCommandTest, ExecuteTogglesAnonymityWhenWindowExists) {
    // Arrange
    EXPECT_CALL(*mockWindowManager, getFirstWindow())
        .WillOnce(Return(mockWindow));
    
    EXPECT_CALL(*mockWindow, toggleWindowAnonymity())
        .Times(1);

    ToggleWindowAnonymityCommand command;

    // Act
    command.execute();
}

TEST_F(ToggleWindowAnonymityCommandTest, ExecuteThrowsWhenNoWindow) {
    // Arrange
    EXPECT_CALL(*mockWindowManager, getFirstWindow())
        .WillOnce(Return(nullptr));

    ToggleWindowAnonymityCommand command;

    // Act & Assert
    EXPECT_THROW(command.execute(), std::runtime_error);
}

TEST_F(ToggleWindowAnonymityCommandTest, UseDebounceReturnsTrue) {
    // Arrange
    ToggleWindowAnonymityCommand command;

    // Act & Assert
    EXPECT_TRUE(command.useDebounce());
} 