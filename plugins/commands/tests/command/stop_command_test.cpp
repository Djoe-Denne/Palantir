#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>
#include "command/stop_command.hpp"
#include "mock/mock_application.hpp"

using namespace palantir::command;
using namespace palantir::test;
using namespace testing;

class StopCommandTest : public Test {
protected:
    void SetUp() override {
        mock_app_ = std::make_shared<MockApplication>("");
        palantir::Application::setInstance(mock_app_);
    }

    void TearDown() override {
        palantir::Application::setInstance(nullptr);
        mock_app_.reset();
    }

    std::shared_ptr<MockApplication> mock_app_;
};

TEST_F(StopCommandTest, Execute_CallsStopOnApplication) {
    // Expect the stop method to be called exactly once
    EXPECT_CALL(*mock_app_, quit()).Times(1);

    StopCommand command;
    command.execute();
}

TEST_F(StopCommandTest, UseDebounce_ReturnsFalse) {
    StopCommand command;
    EXPECT_FALSE(command.useDebounce());
} 