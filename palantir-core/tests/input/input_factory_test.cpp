#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>
#include <fstream>
#include "input/input_factory.hpp"
#include "input/configurable_input.hpp"
#include "mock/mock_key_config.hpp"

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
        
        // Write test configuration to the file
        std::ofstream configFile(tempConfigPath);
        configFile << "[commands]\n";
        configFile << "test.command1 = Ctrl+F1\n";
        configFile << "test.command2 = Alt+A\n";
        configFile << "test.command3 = Shift+Space\n";
        configFile.close();
        
        // Initialize the InputFactory with the test config
        InputFactory::initialize(tempConfigPath.string());
    }

    void TearDown() override {
        // Clean up the temporary file
        if (fs::exists(tempConfigPath)) {
            fs::remove(tempConfigPath);
        }
    }

    fs::path tempConfigPath;
};

TEST_F(InputFactoryTest, Initialize_ValidConfigFile_Succeeds) {
    // Test that initialization with a valid config file succeeds
    // This is implicitly tested in SetUp, but we'll add an explicit test
    EXPECT_NO_THROW(InputFactory::initialize(tempConfigPath.string()));
}

TEST_F(InputFactoryTest, HasShortcut_ExistingCommand_ReturnsTrue) {
    // Test checking if shortcuts exist for existing commands
    EXPECT_TRUE(InputFactory::hasShortcut("test.command1"));
    EXPECT_TRUE(InputFactory::hasShortcut("test.command2"));
    EXPECT_TRUE(InputFactory::hasShortcut("test.command3"));
}

TEST_F(InputFactoryTest, HasShortcut_NonExistentCommand_ReturnsFalse) {
    // Test checking if a shortcut exists for a non-existent command
    EXPECT_FALSE(InputFactory::hasShortcut("non.existent.command"));
}

TEST_F(InputFactoryTest, GetConfiguredCommands_ReturnsAllCommands) {
    // Test getting all configured commands
    std::vector<std::string> commands = InputFactory::getConfiguredCommands();
    
    // Verify that all commands are returned
    EXPECT_THAT(commands, UnorderedElementsAre("test.command1", "test.command2", "test.command3"));
}

TEST_F(InputFactoryTest, CreateInput_ExistingCommand_ReturnsConfiguredInput) {
    // Test creating input for an existing command
    std::unique_ptr<ConfigurableInput> input = InputFactory::createInput("test.command1");
    
    // Verify that the input was created
    EXPECT_NE(input, nullptr);
}

TEST_F(InputFactoryTest, CreateInput_NonExistentCommand_ThrowsException) {
    // Test creating input for a non-existent command
    EXPECT_THROW(InputFactory::createInput("non.existent.command"), std::runtime_error);
}

TEST_F(InputFactoryTest, Initialize_InvalidConfigFile_CreatesDefaultConfig) {
    // Test with a non-existent config file
    fs::path nonExistentPath = fs::temp_directory_path() / "non_existent_config.ini";
    
    // Ensure the file doesn't exist
    if (fs::exists(nonExistentPath)) {
        fs::remove(nonExistentPath);
    }
    
    // Initialize with a non-existent file should create a default config
    EXPECT_NO_THROW(InputFactory::initialize(nonExistentPath.string()));
    
    // Verify that the file was created
    EXPECT_TRUE(fs::exists(nonExistentPath));
    
    // Clean up
    if (fs::exists(nonExistentPath)) {
        fs::remove(nonExistentPath);
    }
} 