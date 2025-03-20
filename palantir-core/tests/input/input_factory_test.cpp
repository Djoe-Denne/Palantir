#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>
#include <fstream>
#include "input/input_factory.hpp"
#include "input/keyboard_Input.hpp"
#include "mock/input/mock_key_register.hpp"
#include "exception/exceptions.hpp"

using namespace palantir::input;
using namespace palantir::test;
using namespace testing;
namespace fs = std::filesystem;

class InputFactoryPublicConstructor : public InputFactory {
public:
    using InputFactory::InputFactory;
};

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
    auto input = inputFactory->createInput("test.command1");
    
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

// Test for line 30 - directory doesn't exist case
TEST_F(InputFactoryTest, Initialize_NonExistentDirectory_CreatesDirectoryAndDefaultConfig) {
    // Create a path with a non-existent directory
    fs::path nonExistentDirPath = fs::temp_directory_path() / "non_existent_dir" / "config.ini";
    
    // Ensure the directory doesn't exist
    fs::path parentDir = nonExistentDirPath.parent_path();
    if (fs::exists(parentDir)) {
        fs::remove_all(parentDir);
    }
    
    // Initialize with a path where the directory doesn't exist
    EXPECT_NO_THROW(inputFactory->initialize(nonExistentDirPath.string()));
    
    // Verify that the directory and file were created
    EXPECT_TRUE(fs::exists(nonExistentDirPath));
    
    // Clean up
    if (fs::exists(parentDir)) {
        fs::remove_all(parentDir);
    }
}

// Test for lines 52-53 - createDefaultConfig throws exception when file can't be created
TEST_F(InputFactoryTest, CreateDefaultConfig_CannotCreateFile_ThrowsException) {
    // Create a new InputFactory instance
    auto factory = InputFactory::getInstance();
    
    // Use a path that cannot be written to (this varies by system, using a special character path)
    fs::path invalidPath = fs::temp_directory_path() / "\0invalid.ini";
    
    // Initialize with an invalid path should throw
    EXPECT_THROW(factory->initialize(invalidPath.string()), palantir::exception::TraceableConfigFileException);
}

// Test for lines 83-84 - createInput throws when key/modifier is invalid
TEST_F(InputFactoryTest, CreateInput_InvalidKeyOrModifier_ThrowsException) {
    // Create a temporary config file with invalid key
    fs::path invalidConfigPath = fs::temp_directory_path() / "invalid_key_config.ini";
    
    std::ofstream configFile(invalidConfigPath.string());
    configFile << "[commands]\n"
               << "test.invalid = InvalidModifier+InvalidKey\n";
    configFile.close();
    
    // Set up expectations for key validation
    ON_CALL(*mockKeyRegister, hasKey(StrEq("INVALIDMODIFIER"))).WillByDefault(Return(false));
    ON_CALL(*mockKeyRegister, hasKey(StrEq("INVALIDKEY"))).WillByDefault(Return(false));
    
    // Initialize with the invalid config
    auto factory = InputFactory::getInstance();
    factory->initialize(invalidConfigPath.string());
    
    // Creating input for the invalid command should throw
    EXPECT_THROW(factory->createInput("test.invalid"), std::invalid_argument);
    
    // Clean up
    if (fs::exists(invalidConfigPath)) {
        fs::remove(invalidConfigPath);
    }
}

// Test for line 95 - hasShortcut throws when InputFactory is not initialized
TEST_F(InputFactoryTest, HasShortcut_NotInitialized_ThrowsException) {
    // Create a new InputFactory instance without initializing it
    auto newFactory = std::make_shared<InputFactoryPublicConstructor>();
    
    // Temporarily set the instance to our new factory
    InputFactory::setInstance(newFactory);
    
    // hasShortcut should throw when factory is not initialized
    EXPECT_THROW(newFactory->hasShortcut("test.command"), palantir::exception::TraceableInputFactoryException);
    
    // Reset the instance
    InputFactory::setInstance(inputFactory);
}

// Test for lines 104-105 - getConfiguredCommands throws when InputFactory is not initialized
TEST_F(InputFactoryTest, GetConfiguredCommands_NotInitialized_ThrowsException) {
    // Create a new InputFactory instance without initializing it
    auto newFactory = std::make_shared<InputFactoryPublicConstructor>();
    
    // Temporarily set the instance to our new factory
    InputFactory::setInstance(newFactory);
    
    // getConfiguredCommands should throw when factory is not initialized
    EXPECT_THROW(newFactory->getConfiguredCommands(), palantir::exception::TraceableInputFactoryException);
    
    // Reset the instance
    InputFactory::setInstance(inputFactory);
}

// Test for lines 112-113 - setInstance sets the singleton instance
TEST_F(InputFactoryTest, SetInstance_NewInstance_InstanceIsSet) {
    // Create a new InputFactory instance
    auto newFactory = std::make_shared<InputFactoryPublicConstructor>();
    
    // Set the instance to our new factory
    InputFactory::setInstance(newFactory);
    
    // Get the instance and verify it's our new factory
    auto instance = InputFactory::getInstance();
    EXPECT_EQ(instance, newFactory);
    
    // Reset the instance
    InputFactory::setInstance(inputFactory);
} 