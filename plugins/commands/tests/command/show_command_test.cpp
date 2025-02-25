#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>
#include "command/show_command.hpp"
#include "../mocks/mock_application.hpp"
#include "../mocks/mock_window_manager.hpp"
#include "../mocks/mock_window.hpp"

using namespace interview_cheater::command;
using namespace interview_cheater::test;
using namespace testing;

class ShowCommandTest : public Test {
protected:
    void SetUp() override {
        mock_app_ = std::make_shared<MockApplication>("");
        mock_window_ = std::make_shared<MockWindow>();
        mock_window_manager_ = std::make_shared<MockWindowManager>();
        
        // Set up expectations for window manager
        EXPECT_CALL(*mock_window_manager_, getFirstWindow())
            .WillRepeatedly(Return(mock_window_));
        EXPECT_CALL(*mock_app_, getWindowManager())
            .WillRepeatedly(Return(mock_window_manager_));

        interview_cheater::Application::setInstance(mock_app_);
        interview_cheater::window::WindowManager::setInstance(mock_window_manager_);
    }

    void TearDown() override {
        interview_cheater::Application::setInstance(nullptr);
        interview_cheater::window::WindowManager::setInstance(nullptr);
        mock_app_.reset();
        mock_window_.reset();
        mock_window_manager_.reset();
    }

    std::shared_ptr<MockApplication> mock_app_;
    std::shared_ptr<MockWindow> mock_window_;
    std::shared_ptr<MockWindowManager> mock_window_manager_;
};

TEST_F(ShowCommandTest, Execute_CallsShowOnWindow) {
    // Set up expectations
    EXPECT_CALL(*mock_window_, show()).Times(1);

    ShowCommand command;
    command.execute();
}

TEST_F(ShowCommandTest, Execute_TrueOnHasDebounce) {
    ShowCommand command;
    EXPECT_TRUE(command.useDebounce());
}



