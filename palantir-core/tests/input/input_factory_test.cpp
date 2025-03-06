#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>
#include <fstream>
#include "input/input_factory.hpp"
#include "input/configurable_input.hpp"
#include "mock/input/mock_key_register.hpp"

using namespace palantir::input;
using namespace palantir::test;
using namespace testing;
namespace fs = std::filesystem;

// Test fixture for InputFactory tests
class InputFactoryTest : public Test {
protected:
    void SetUp() override {
        // Create a temporary config file for testing
        tempConfigPath = fs::temp_directory_path() / "test_input_factory.ini";
        
        // Fill the config file with test data
        std::ofstream configFile(tempConfigPath.string());
        configFile << "[commands]\n"
                   << "test.command1 = Ctrl+1\n"
                   << "test.command2 = Ctrl+2\n"
                   << "test.command3 = Ctrl+3\n";

        configFile.close();

        mockKeyRegister = std::make_shared<MockKeyRegister>();
        ON_CALL(*mockKeyRegister, hasKey(StrEq("CTRL"))).WillByDefault(Return(true));
        ON_CALL(*mockKeyRegister, hasKey(StrEq("1"))).WillByDefault(Return(true));

        KeyRegister::setInstance(mockKeyRegister);
        // Initialize the InputFactory with the test config
        inputFactory = InputFactory::getInstance();
        inputFactory->initialize(tempConfigPath.string());

    }

    void TearDown() override {
        // Clean up the temporary file
        if (fs::exists(tempConfigPath)) {
            fs::remove(tempConfigPath);
        }

        KeyRegister::setInstance(nullptr);

        mockKeyRegister.reset();
    }

    fs::path tempConfigPath;
    std::shared_ptr<InputFactory> inputFactory;
    std::shared_ptr<MockKeyRegister> mockKeyRegister;
};

TEST_F(InputFactoryTest, HasShortcut_ExistingCommand_ReturnsTrue) {
    // Test checking if shortcuts exist for existing commands
    EXPECT_TRUE(inputFactory->hasShortcut("test.command1"));
    EXPECT_TRUE(inputFactory->hasShortcut("test.command2"));
    EXPECT_TRUE(inputFactory->hasShortcut("test.command3"));
}

TEST_F(InputFactoryTest, HasShortcut_NonExistentCommand_ReturnsFalse) {
    // Test checking if a shortcut exists for a non-existent command
    EXPECT_FALSE(inputFactory->hasShortcut("non.existent.command"));
}

TEST_F(InputFactoryTest, GetConfiguredCommands_ReturnsAllCommands) {
    // Test getting all configured commands
    std::vector<std::string> commands = inputFactory->getConfiguredCommands();
    
    // Verify that all commands are returned
    EXPECT_THAT(commands, UnorderedElementsAre("test.command1", "test.command2", "test.command3"));
}

TEST_F(InputFactoryTest, CreateInput_ExistingCommand_ReturnsConfiguredInput) {
    // Test creating input for an existing command
    std::unique_ptr<ConfigurableInput> input = inputFactory->createInput("test.command1");
    
    // Verify that the input was created
    EXPECT_NE(input, nullptr);
}

TEST_F(InputFactoryTest, CreateInput_NonExistentCommand_ThrowsException) {
    // Test creating input for a non-existent command
    EXPECT_THROW(inputFactory->createInput("non.existent.command"), std::runtime_error);
}

TEST_F(InputFactoryTest, Initialize_InvalidConfigFile_CreatesDefaultConfig) {
    // Test with a non-existent config file
    fs::path nonExistentPath = fs::temp_directory_path() / "non_existent_config.ini";
    
    // Ensure the file doesn't exist
    if (fs::exists(nonExistentPath)) {
        fs::remove(nonExistentPath);
    }
    
    // Initialize with a non-existent file should create a default config
    EXPECT_NO_THROW(inputFactory->initialize(nonExistentPath.string()));
    
    // Verify that the file was created
    EXPECT_TRUE(fs::exists(nonExistentPath));
    
    // Clean up
    if (fs::exists(nonExistentPath)) {
        fs::remove(nonExistentPath);
    }
} 