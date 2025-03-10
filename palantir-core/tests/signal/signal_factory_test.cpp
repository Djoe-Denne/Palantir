#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <vector>
#include <string>

#include "signal/signal_factory.hpp"
#include "input/input_factory.hpp"
#include "command/command_factory.hpp"
#include "mock/mock_application.hpp"
#include "mock/input/mock_configurable_input.hpp"
#include "mock/input/mock_input_factory.hpp"
#include "mock/command/mock_command.hpp"
#include "mock/command/mock_command_factory.hpp"
#include "mock/signal/mock_signal_factory.hpp"
using namespace palantir::signal;
using namespace palantir::input;
using namespace palantir::command;
using namespace palantir::test;
using namespace testing;

class SignalFactoryTest : public Test {
protected:
    void SetUp() override {
        mockApp = std::make_shared<MockApplication>("");
        mockInputFactory = std::make_shared<MockInputFactory>();
        mockCommandFactory = std::make_shared<MockCommandFactory>();

        InputFactory::setInstance(mockInputFactory);
        CommandFactory::setInstance(mockCommandFactory);
    }

    void TearDown() override {
        InputFactory::setInstance(nullptr);
        CommandFactory::setInstance(nullptr);

        mockApp.reset();
        mockInputFactory.reset();
        mockCommandFactory.reset();
    }

    std::shared_ptr<SignalFactory> signalFactory = SignalFactory::getInstance();

    std::shared_ptr<MockApplication> mockApp;
    std::shared_ptr<MockInputFactory> mockInputFactory;
    std::shared_ptr<MockCommandFactory> mockCommandFactory;
};

TEST_F(SignalFactoryTest, CreateSignals_NoCommands_ReturnsEmptyVector) {
    // Mock InputFactory to return empty command list
    EXPECT_CALL(*mockInputFactory, getConfiguredCommands())
        .WillOnce(Return(std::vector<std::string>()));

    auto signals = signalFactory->createSignals(mockApp);
    EXPECT_TRUE(signals.empty());
}

TEST_F(SignalFactoryTest, CreateSignals_WithValidCommand_ReturnsSignal) {
    std::vector<std::string> commands = {"test_command"};
    std::unique_ptr<MockConfigurableInput> mockInput = std::make_unique<MockConfigurableInput>(0,0);
    std::unique_ptr<MockCommand> mockCommand = std::make_unique<MockCommand>();
    
    EXPECT_CALL(*mockCommand, useDebounce())
        .WillOnce(Return(false));

    // Mock InputFactory
    EXPECT_CALL(*mockInputFactory, getConfiguredCommands())
        .WillOnce(Return(commands));
    EXPECT_CALL(*mockInputFactory, createInput("test_command"))
        .WillOnce(Return(std::move(mockInput)));

    // Mock CommandFactory
    EXPECT_CALL(*mockCommandFactory, getCommand("test_command"))
        .WillOnce(Return(std::move(mockCommand)));

    auto signals = signalFactory->createSignals(mockApp);
    EXPECT_EQ(signals.size(), 1);
    EXPECT_NE(signals[0], nullptr);
}

TEST_F(SignalFactoryTest, CreateSignals_WithInvalidCommand_ThrowsException) {
    std::vector<std::string> commands = {"invalid_command"};
    
    // Mock InputFactory
    EXPECT_CALL(*mockInputFactory, getConfiguredCommands())
        .WillOnce(Return(commands));

    // Mock CommandFactory to return nullptr for invalid command
    EXPECT_CALL(*mockCommandFactory, getCommand("invalid_command"))
        .WillOnce(Return(nullptr));

    EXPECT_THROW(signalFactory->createSignals(mockApp), std::runtime_error);
}

TEST_F(SignalFactoryTest, CreateSignals_MultipleCommands_ReturnsMultipleSignals) {
    std::vector<std::string> commands = {"command1", "command2"};
    std::unique_ptr<MockConfigurableInput> mockInput1 = std::make_unique<MockConfigurableInput>(0,0);
    std::unique_ptr<MockConfigurableInput> mockInput2 = std::make_unique<MockConfigurableInput>(0,0);
    std::unique_ptr<MockCommand> mockCommand1 = std::make_unique<MockCommand>();
    std::unique_ptr<MockCommand> mockCommand2 = std::make_unique<MockCommand>();
    
    EXPECT_CALL(*mockCommand1, useDebounce())
        .WillOnce(Return(false));
    EXPECT_CALL(*mockCommand2, useDebounce())
        .WillOnce(Return(false));
    // Mock InputFactory
    EXPECT_CALL(*mockInputFactory, getConfiguredCommands())
        .WillOnce(Return(commands));
    EXPECT_CALL(*mockInputFactory, createInput(_))
        .Times(2)
        .WillOnce(Return(std::move(mockInput1)))
        .WillOnce(Return(std::move(mockInput2)));

    // Mock CommandFactory
    EXPECT_CALL(*mockCommandFactory, getCommand(_))
        .Times(2)
        .WillOnce(Return(std::move(mockCommand1)))
        .WillOnce(Return(std::move(mockCommand2)));

    auto signals = signalFactory->createSignals(mockApp);
    EXPECT_EQ(signals.size(), 2);
    EXPECT_NE(signals[0], nullptr);
    EXPECT_NE(signals[1], nullptr);
} 