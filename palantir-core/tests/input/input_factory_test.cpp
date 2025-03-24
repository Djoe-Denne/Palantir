#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>
#include <fstream>
#include "input/keyboard_input_factory.hpp"
#include "input/keyboard_Input.hpp"
#include "mock/input/mock_key_register.hpp"
#include "exception/exceptions.hpp"
#include "config/desktop_config.hpp"

using namespace palantir::input;
using namespace palantir::test;
using namespace palantir::config;
using namespace testing;
namespace fs = std::filesystem;

class DesktopTestConfig : public Config {
public:
    DesktopTestConfig() = default;
    ~DesktopTestConfig() override = default;

    DesktopTestConfig(const DesktopTestConfig&) = delete;
    auto operator=(const DesktopTestConfig&) -> DesktopTestConfig& = delete;

    DesktopTestConfig(DesktopTestConfig&&) = delete;
    auto operator=(DesktopTestConfig&&) -> DesktopTestConfig& = delete;

    [[nodiscard]] virtual auto getShortcutsPath() const -> std::filesystem::path {
        return tempConfigPath;
    }

    auto setShortcutsPath(const fs::path& path) -> void {
        tempConfigPath = path;
    }

private:
    fs::path tempConfigPath;
};


// Test fixture for KeyboardInputFactory tests
class KeyboardInputFactoryTest : public Test {
protected:
    void SetUp() override {
        auto config = DesktopTestConfig();
        config.setShortcutsPath(fs::temp_directory_path() / "test_input_factory.ini");
        // Fill the config file with test data
        std::ofstream configFile(config.getShortcutsPath().string());
        configFile << "[commands]\n"
                   << "test.command1 = Ctrl+1\n"
                   << "test.command2 = Ctrl+2\n"
                   << "test.command3 = Ctrl+3\n";

        configFile.close();

        mockKeyRegister = std::make_shared<MockKeyRegister>();
        ON_CALL(*mockKeyRegister, hasKey(StrEq("CTRL"))).WillByDefault(Return(true));
        ON_CALL(*mockKeyRegister, hasKey(StrEq("1"))).WillByDefault(Return(true));

        KeyRegister::setInstance(mockKeyRegister);
        inputFactory = std::make_shared<KeyboardInputFactory>(config);
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
    std::shared_ptr<KeyboardInputFactory> inputFactory;
    std::shared_ptr<MockKeyRegister> mockKeyRegister;
};

TEST_F(KeyboardInputFactoryTest, HasShortcut_ExistingCommand_ReturnsTrue) {
    // Test checking if shortcuts exist for existing commands
    EXPECT_TRUE(inputFactory->hasShortcut("test.command1"));
    EXPECT_TRUE(inputFactory->hasShortcut("test.command2"));
    EXPECT_TRUE(inputFactory->hasShortcut("test.command3"));
}

TEST_F(KeyboardInputFactoryTest, HasShortcut_NonExistentCommand_ReturnsFalse) {
    // Test checking if a shortcut exists for a non-existent command
    EXPECT_FALSE(inputFactory->hasShortcut("non.existent.command"));
}

TEST_F(KeyboardInputFactoryTest, GetConfiguredCommands_ReturnsAllCommands) {
    // Test getting all configured commands
    std::vector<std::string> commands = inputFactory->getConfiguredCommands();
    
    // Verify that all commands are returned
    EXPECT_THAT(commands, UnorderedElementsAre("test.command1", "test.command2", "test.command3"));
}

TEST_F(KeyboardInputFactoryTest, CreateInput_ExistingCommand_ReturnsConfiguredInput) {
    // Test creating input for an existing command
    std::unique_ptr<KeyboardInput> input = inputFactory->createInput("test.command1");
    
    // Verify that the input was created
    EXPECT_NE(input, nullptr);
}

TEST_F(KeyboardInputFactoryTest, CreateInput_NonExistentCommand_ThrowsException) {
    // Test creating input for a non-existent command
    EXPECT_THROW(inputFactory->createInput("non.existent.command"), std::runtime_error);
}

TEST_F(KeyboardInputFactoryTest, Initialize_InvalidConfigFile_CreatesDefaultConfig) {
    // Test with a non-existent config file
    fs::path nonExistentPath = fs::temp_directory_path() / "non_existent_config.ini";
    
    // Ensure the file doesn't exist
    if (fs::exists(nonExistentPath)) {
        fs::remove(nonExistentPath);
    }

    auto config = DesktopTestConfig();
    config.setShortcutsPath(nonExistentPath);
    
    // Initialize with a non-existent file should create a default config
    EXPECT_NO_THROW(std::make_shared<KeyboardInputFactory>(config));
    
    // Verify that the file was created
    EXPECT_TRUE(fs::exists(nonExistentPath));
    
    // Clean up
    if (fs::exists(nonExistentPath)) {
        fs::remove(nonExistentPath);
    }
}

// Test for line 30 - directory doesn't exist case
TEST_F(KeyboardInputFactoryTest, Initialize_NonExistentDirectory_CreatesDirectoryAndDefaultConfig) {
    // Create a path with a non-existent directory
    fs::path nonExistentDirPath = fs::temp_directory_path() / "non_existent_dir" / "config.ini";
    
    // Ensure the directory doesn't exist
    fs::path parentDir = nonExistentDirPath.parent_path();
    if (fs::exists(parentDir)) {
        fs::remove_all(parentDir);
    }

    auto config = DesktopTestConfig();
    config.setShortcutsPath(nonExistentDirPath);
    
    // Initialize with a path where the directory doesn't exist
    EXPECT_NO_THROW(std::make_shared<KeyboardInputFactory>(config));
    
    // Verify that the directory and file were created
    EXPECT_TRUE(fs::exists(nonExistentDirPath));
    
    // Clean up
    if (fs::exists(parentDir)) {
        fs::remove_all(parentDir);
    }
}

// Test for lines 52-53 - createDefaultConfig throws exception when file can't be created
TEST_F(KeyboardInputFactoryTest, CreateDefaultConfig_CannotCreateFile_ThrowsException) {    
    // Use a path that cannot be written to (this varies by system, using a special character path)
    fs::path invalidPath = fs::temp_directory_path() / "\0invalid.ini";

    auto config = DesktopTestConfig();
    config.setShortcutsPath(invalidPath);
    
    // Initialize with an invalid path should throw
    EXPECT_THROW(std::make_shared<KeyboardInputFactory>(config), palantir::exception::TraceableConfigFileException);
}

// Test for lines 83-84 - createInput throws when key/modifier is invalid
TEST_F(KeyboardInputFactoryTest, CreateInput_InvalidKeyOrModifier_ThrowsException) {
    // Create a temporary config file with invalid key
    fs::path invalidConfigPath = fs::temp_directory_path() / "invalid_key_config.ini";
    
    std::ofstream configFile(invalidConfigPath.string());
    configFile << "[commands]\n"
               << "test.invalid = InvalidModifier+InvalidKey\n";
    configFile.close();
    
    // Set up expectations for key validation
    ON_CALL(*mockKeyRegister, hasKey(StrEq("INVALIDMODIFIER"))).WillByDefault(Return(false));
    ON_CALL(*mockKeyRegister, hasKey(StrEq("INVALIDKEY"))).WillByDefault(Return(false));
    
    auto config = DesktopTestConfig();
    config.setShortcutsPath(invalidConfigPath);
    
    // Creating input for the invalid command should throw
    EXPECT_THROW(std::make_shared<KeyboardInputFactory>(config), std::invalid_argument);
    
    // Clean up
    if (fs::exists(invalidConfigPath)) {
        fs::remove(invalidConfigPath);
    }
}
