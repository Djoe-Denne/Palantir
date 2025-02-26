#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "command/toggle_window_anonymity_command.hpp"
#include "../mocks/mock_application.hpp"
#include "../mocks/mock_window_manager.hpp"
#include "../mocks/mock_window.hpp"

using namespace interview_cheater;
using namespace interview_cheater::command;
using namespace interview_cheater::test;
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
            
        interview_cheater::Application::setInstance(mockApp);
        interview_cheater::window::WindowManager::setInstance(mockWindowManager);
    }

    void TearDown() override {
        interview_cheater::Application::setInstance(nullptr);
        interview_cheater::window::WindowManager::setInstance(nullptr);
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