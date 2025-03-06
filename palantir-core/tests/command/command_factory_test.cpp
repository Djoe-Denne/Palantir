#include <gtest/gtest.h>
#include <memory>
#include "command/command_factory.hpp"
#include "command/icommand.hpp"
#include "mock/command/mock_command.hpp"

using namespace palantir::command;
using namespace palantir::test;
using namespace testing;

class CommandFactoryTest : public Test {
protected:
    void SetUp() override {
        // Store the original instance
        originalInstance_ = CommandFactory::getInstance();
        // Reset the instance for each test
        CommandFactory::setInstance(nullptr);
    }

    void TearDown() override {
        // Restore the original instance
        CommandFactory::setInstance(originalInstance_);
    }

    std::shared_ptr<CommandFactory> originalInstance_;
};

// Test singleton behavior
TEST_F(CommandFactoryTest, GetInstanceReturnsSameInstance) {
    auto instance1 = CommandFactory::getInstance();
    auto instance2 = CommandFactory::getInstance();
    
    ASSERT_NE(nullptr, instance1);
    ASSERT_EQ(instance1, instance2);
}

// Test command registration and retrieval
TEST_F(CommandFactoryTest, RegisterAndGetCommand) {
    auto factory = CommandFactory::getInstance();
    
    // Register a command
    factory->registerCommand("mock", []() -> std::unique_ptr<ICommand> {
        return std::make_unique<MockCommand>();
    });
    
    // Get the command
    auto cmd = factory->getCommand("mock");
    ASSERT_NE(nullptr, cmd);
}

// Test unregistering commands
TEST_F(CommandFactoryTest, UnregisterCommand) {
    auto factory = CommandFactory::getInstance();
    
    // Register and then unregister
    factory->registerCommand("mock", []() -> std::unique_ptr<ICommand> {
        return std::make_unique<MockCommand>();
    });
    
    ASSERT_TRUE(factory->unregisterCommand("mock"));
    ASSERT_FALSE(factory->unregisterCommand("mock")); // Second attempt should fail
    
    // Verify command is no longer available
    ASSERT_EQ(nullptr, factory->getCommand("mock"));
}

// Test getting non-existent command
TEST_F(CommandFactoryTest, GetNonExistentCommand) {
    auto factory = CommandFactory::getInstance();
    ASSERT_EQ(nullptr, factory->getCommand("non_existent"));
}

// Test registering same command twice
TEST_F(CommandFactoryTest, RegisterSameCommandTwice) {
    auto factory = CommandFactory::getInstance();
    auto creator = []() -> std::unique_ptr<ICommand> {
        return std::make_unique<MockCommand>();
    };
    
    factory->registerCommand("mock", creator);
    // Second registration should override the first one
    factory->registerCommand("mock", creator);
    
    auto cmd = factory->getCommand("mock");
    ASSERT_NE(nullptr, cmd);
}

// Test unregistering non-existent command
TEST_F(CommandFactoryTest, UnregisterNonExistentCommand) {
    auto factory = CommandFactory::getInstance();
    ASSERT_FALSE(factory->unregisterCommand("non_existent"));
}

// Test empty command name
TEST_F(CommandFactoryTest, RegisterEmptyCommandName) {
    auto factory = CommandFactory::getInstance();
    factory->registerCommand("", []() -> std::unique_ptr<ICommand> {
        return std::make_unique<MockCommand>();
    });
    
    ASSERT_NE(nullptr, factory->getCommand(""));
}
