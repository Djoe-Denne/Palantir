#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <any>
#include <memory>

#include "signal/input_signal.hpp"
#include "mock/input/mock_keyboard_Input.hpp"
#include "mock/command/mock_command.hpp"

using namespace palantir::input;
using namespace palantir::command;
using namespace palantir::signal;
using namespace palantir::test;
using namespace testing;

class SignalTest : public Test {
protected:
    void SetUp() override {
        mockInput = std::make_shared<MockKeyboardInput>(0,0);
        mockCommand = std::make_shared<MockCommand>();
        signal = std::make_unique<InputSignal>(
            std::unique_ptr<IInput>(mockInput.get()),
            std::unique_ptr<ICommand>(mockCommand.get()),
            false
        );
    }

    void TearDown() override {
        signal.release();
    }

    std::shared_ptr<MockKeyboardInput> mockInput;
    std::shared_ptr<MockCommand> mockCommand;
    std::unique_ptr<InputSignal> signal;
    std::any emptyEvent;
};

TEST_F(SignalTest, Constructor_ValidInputs_CreatesInstance) {
    EXPECT_NE(signal, nullptr);
}

TEST_F(SignalTest, Start_SetsActiveState) {
    signal->start();
    EXPECT_TRUE(signal->isActive());
}

TEST_F(SignalTest, Stop_ClearsActiveState) {
    signal->start();
    signal->stop();
    EXPECT_FALSE(signal->isActive());
}

TEST_F(SignalTest, Check_InactiveSignal_DoesNotExecuteCommand) {
    EXPECT_CALL(*mockInput, isActive(_)).Times(0);
    EXPECT_CALL(*mockCommand, execute()).Times(0);

    signal->check(emptyEvent);
}

TEST_F(SignalTest, Check_ActiveSignalWithActiveInput_ExecutesCommand) {
    signal->start();

    EXPECT_CALL(*mockInput, isActive(_)).WillOnce(Return(true));
    EXPECT_CALL(*mockCommand, execute()).Times(1);

    signal->check(emptyEvent);
}

TEST_F(SignalTest, Check_ActiveSignalWhileInactive_DoesNotExecuteCommand) {
    signal->start();

    EXPECT_CALL(*mockInput, isActive(_)).WillOnce(Return(false));
    EXPECT_CALL(*mockCommand, execute()).Times(0);

    signal->check(emptyEvent);
}

TEST_F(SignalTest, Check_WithDebounce_LimitsCommandExecution) {

    auto mockInput1 = std::make_unique<MockKeyboardInput>(0,0);
    auto mockCommand1 = std::make_unique<MockCommand>();
    EXPECT_CALL(*mockInput1, isActive(_)).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(*mockCommand1, execute()).Times(1);  // Should only execute once due to debounce

    auto debouncedSignal = std::make_unique<InputSignal>(
        std::move(mockInput1),
        std::move(mockCommand1),
        true
    );
    debouncedSignal->start();

    debouncedSignal->check(emptyEvent);
    debouncedSignal->check(emptyEvent);  // Second check within debounce period
} 