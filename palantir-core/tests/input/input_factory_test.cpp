#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>
#include <fstream>
#include "input/input_factory.hpp"
#include "input/keyboard_Input.hpp"
#include "mock/input/mock_key_register.hpp"
#include "exception/exceptions.hpp"
#include "config/config.hpp"

using namespace palantir::input;
using namespace palantir::config;
using namespace palantir::test;
using namespace testing;
namespace fs = std::filesystem;

class TestConfig : public Config {
public:
    ~TestConfig() override = default;
    [[nodiscard]] auto getConfigPath() const -> std::filesystem::path override {
        return path_;
    }

    [[nodiscard]] auto getConfigurationFormat() const -> std::string override {
        return "ini";
    }
    
    void setConfigPath(const std::filesystem::path& path) {
        path_ = path;
    }

private:
    std::filesystem::path path_;
};

// Test fixture for InputFactory tests
class InputFactoryTest : public Test {
protected:
    void SetUp() override {
        config = std::make_shared<TestConfig>();
        // Create a temporary config file for testing
        config->setConfigPath(fs::temp_directory_path() / "input_factory_test");
        
        std::filesystem::create_directories(config->getConfigPath());
        // Fill the config file with test data
        std::ofstream configFile((config->getConfigPath() / "shortcuts.ini").string());
        configFile << "[commands]\n"
                   << "test.command1 = Ctrl+1\n"
                   << "test.command2 = Ctrl+2\n"
                   << "test.command3 = Ctrl+3\n";

        configFile.close();

        mockKeyRegister = std::make_shared<MockKeyRegister>();
        ON_CALL(*mockKeyRegister, hasKey(StrEq("CTRL"))).WillByDefault(Return(true));
        ON_CALL(*mockKeyRegister, hasKey(StrEq("1"))).WillByDefault(Return(true));

        KeyRegister::setInstance(mockKeyRegister);
        inputFactory = std::make_shared<InputFactory>(config);
        inputFactory->initialize();
    }

    void TearDown() override {
        // Clean up the temporary file
        if (fs::exists(config->getConfigPath())) {
            fs::remove_all(config->getConfigPath());
        }

        KeyRegister::setInstance(nullptr);

        mockKeyRegister.reset();
    }

    std::shared_ptr<TestConfig> config;
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
    std::unique_ptr<IInput> input = inputFactory->createInput("test.command1");
    
    // Verify that the input was created
    EXPECT_NE(input, nullptr);
}

TEST_F(InputFactoryTest, CreateInput_NonExistentCommand_ThrowsException) {
    // Test creating input for a non-existent command
    EXPECT_THROW(inputFactory->createInput("non.existent.command"), palantir::exception::TraceableShortcutConfigurationException);
}

// Test for line 30 - directory doesn't exist case
TEST_F(InputFactoryTest, Initialize_NonExistentDirectory_CreatesDirectoryAndDefaultConfig) {
    // Create a path with a non-existent directory
    fs::path nonExistentDirPath = fs::temp_directory_path() / "input_factory_test" / "non_existent_dir";
    
    // Ensure the directory doesn't exist
    fs::path parentDir = nonExistentDirPath.parent_path();
    if (fs::exists(parentDir)) {
        fs::remove_all(parentDir);
    }

    config->setConfigPath(nonExistentDirPath);
    
    // Initialize with a path where the directory doesn't exist
    EXPECT_NO_THROW(inputFactory->initialize());
    
    // Verify that the directory and file were created
    EXPECT_TRUE(fs::exists(nonExistentDirPath));
    
    // Clean up
    if (fs::exists(parentDir)) {
        fs::remove_all(parentDir);
    }
}

// Test for lines 83-84 - createInput throws when key/modifier is invalid
TEST_F(InputFactoryTest, CreateInput_InvalidKeyOrModifier_ThrowsException) {
    // Create a temporary config file with invalid key
    fs::path invalidConfigPath = fs::temp_directory_path() / "input_factory_test" / "invalid_key_config" / "shortcuts.ini";
    std::filesystem::create_directories(invalidConfigPath.parent_path());

    std::ofstream configFile(invalidConfigPath.string());
    configFile << "[commands]\n"
               << "test.invalid = InvalidModifier+InvalidKey\n";
    configFile.close();

    config->setConfigPath(invalidConfigPath.parent_path());
    
    // Set up expectations for key validation
    ON_CALL(*mockKeyRegister, hasKey(StrEq("INVALIDMODIFIER"))).WillByDefault(Return(false));
    ON_CALL(*mockKeyRegister, hasKey(StrEq("INVALIDKEY"))).WillByDefault(Return(false));
    
    inputFactory->initialize();
    
    // Creating input for the invalid command should throw
    EXPECT_THROW(inputFactory->createInput("test.invalid"), std::invalid_argument);
    
    // Clean up
    if (fs::exists(invalidConfigPath)) {
        fs::remove_all(invalidConfigPath);
    }
}

// Test for line 95 - hasShortcut throws when InputFactory is not initialized
TEST_F(InputFactoryTest, HasShortcut_NotInitialized_ThrowsException) {
    // Create a new InputFactory instance without initializing it
    auto newFactory = std::make_shared<InputFactory>(config);
    
    // hasShortcut should throw when factory is not initialized
    EXPECT_THROW(newFactory->hasShortcut("test.command"), palantir::exception::TraceableInputFactoryException);
    
}

// Test for lines 104-105 - getConfiguredCommands throws when InputFactory is not initialized
TEST_F(InputFactoryTest, GetConfiguredCommands_NotInitialized_ThrowsException) {
    // Create a new InputFactory instance without initializing it
    auto newFactory = std::make_shared<InputFactory>(config);
    
    // getConfiguredCommands should throw when factory is not initialized
    EXPECT_THROW(newFactory->getConfiguredCommands(), palantir::exception::TraceableInputFactoryException);
    
}
