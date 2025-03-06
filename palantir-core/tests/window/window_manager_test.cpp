#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "window/window_manager.hpp"
#include "mock/window/mock_window.hpp"
#include "mock/command/mock_command.hpp"

using namespace palantir::window;
using namespace palantir::test;
using namespace testing;

class WindowManagerTest : public Test {
protected:
    void SetUp() override {
        windowManager = WindowManager::getInstance();
        mockWindow = std::make_shared<MockWindow>();
        mockCommand = std::make_unique<MockCommand>();
    }

    void TearDown() override {
        WindowManager::setInstance(nullptr);
        mockWindow.reset();
        mockCommand.reset();
    }

    std::shared_ptr<WindowManager> windowManager;
    std::shared_ptr<MockWindow> mockWindow;
    std::unique_ptr<MockCommand> mockCommand;
};

TEST_F(WindowManagerTest, GetInstance_ReturnsSameInstance) {
    auto instance1 = WindowManager::getInstance();
    auto instance2 = WindowManager::getInstance();
    EXPECT_EQ(instance1, instance2);
}

TEST_F(WindowManagerTest, AddWindow_WindowAdded_CanBeRetrieved) {
    windowManager->addWindow(mockWindow);
    EXPECT_EQ(windowManager->getFirstWindow(), mockWindow);
}

TEST_F(WindowManagerTest, RemoveWindow_WindowRemoved_CannotBeRetrieved) {
    windowManager->addWindow(mockWindow);
    windowManager->removeWindow(mockWindow.get());
    EXPECT_EQ(windowManager->getFirstWindow(), nullptr);
}

TEST_F(WindowManagerTest, GetFirstWindow_NoWindows_ReturnsNullptr) {
    EXPECT_EQ(windowManager->getFirstWindow(), nullptr);
}

TEST_F(WindowManagerTest, GetFirstWindow_MultipleWindows_ReturnsFirstAdded) {
    auto mockWindow2 = std::make_shared<MockWindow>();
    
    windowManager->addWindow(mockWindow);
    windowManager->addWindow(mockWindow2);
    
    EXPECT_EQ(windowManager->getFirstWindow(), mockWindow);
}

TEST_F(WindowManagerTest, HasRunningWindows_NoWindows_ReturnsFalse) {
    EXPECT_FALSE(windowManager->hasRunningWindows());
}

TEST_F(WindowManagerTest, HasRunningWindows_WindowNotRunning_ReturnsFalse) {
    EXPECT_CALL(*mockWindow, isRunning())
        .WillOnce(Return(false));
    
    windowManager->addWindow(mockWindow);
    EXPECT_FALSE(windowManager->hasRunningWindows());
}

TEST_F(WindowManagerTest, HasRunningWindows_WindowRunning_ReturnsTrue) {
    EXPECT_CALL(*mockWindow, isRunning())
        .WillOnce(Return(true));
    
    windowManager->addWindow(mockWindow);
    EXPECT_TRUE(windowManager->hasRunningWindows());
}

TEST_F(WindowManagerTest, HasRunningWindows_MultipleWindows_ReturnsTrue) {
    auto mockWindow2 = std::make_shared<MockWindow>();
    
    EXPECT_CALL(*mockWindow, isRunning())
        .WillOnce(Return(false));
    EXPECT_CALL(*mockWindow2, isRunning())
        .WillOnce(Return(true));
    
    windowManager->addWindow(mockWindow);
    windowManager->addWindow(mockWindow2);
    EXPECT_TRUE(windowManager->hasRunningWindows());
}

TEST_F(WindowManagerTest, ExecuteCommand_ValidCommand_CommandExecuted) {
    EXPECT_CALL(*mockCommand, execute())
        .Times(1);
    
    windowManager->executeCommand(std::move(mockCommand));
}

TEST_F(WindowManagerTest, ExecuteCommand_NullCommand_NoExecution) {
    windowManager->executeCommand(nullptr);
    // Test passes if no crash occurs
}
