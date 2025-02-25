#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>
#include "command/stop_command.hpp"
#include "../mocks/mock_application.hpp"

using namespace interview_cheater::command;
using namespace interview_cheater::test;
using namespace testing;

class StopCommandTest : public Test {
protected:
    void SetUp() override {
        mock_app_ = std::make_shared<MockApplication>("");
        interview_cheater::Application::setInstance(mock_app_);
    }

    void TearDown() override {
        interview_cheater::Application::setInstance(nullptr);
        mock_app_.reset();
    }

    std::shared_ptr<MockApplication> mock_app_;
};

TEST_F(StopCommandTest, Execute_CallsStopOnApplication) {
    // Expect the stop method to be called exactly once
    EXPECT_CALL(*mock_app_, quit()).Times(1);

    StopCommand command;
    command.execute();
    EXPECT_TRUE(Mock::VerifyAndClearExpectations(mock_app_.get()));
}

TEST_F(StopCommandTest, UseDebounce_ReturnsFalse) {
    StopCommand command;
    EXPECT_FALSE(command.useDebounce());
} 