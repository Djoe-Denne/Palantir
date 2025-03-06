#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>
#include <fstream>
#include "input/key_config.hpp"

using namespace palantir::input;
using namespace testing;
namespace fs = std::filesystem;

class KeyConfigTest : public Test {
protected:
    void SetUp() override {
        // Create a temporary config file for testing
        tempConfigPath = fs::temp_directory_path() / "test_key_config.ini";
        
        // Write test configuration to the file
        std::ofstream configFile(tempConfigPath);
        configFile << "[commands]\n";
        configFile << "test.command1 = Ctrl+F1\n";
        configFile << "test.command2 = Alt+A\n";
        configFile << "test.command3 = Shift+Space\n";
        configFile.close();
        
        // Create the KeyConfig instance with the test file
        keyConfig = std::make_unique<KeyConfig>(tempConfigPath.string());
    }

    void TearDown() override {
        // Clean up the temporary file
        keyConfig.reset();
        if (fs::exists(tempConfigPath)) {
            fs::remove(tempConfigPath);
        }
    }

    fs::path tempConfigPath;
    std::unique_ptr<KeyConfig> keyConfig;
};

TEST_F(KeyConfigTest, Constructor_ValidConfigFile_LoadsShortcuts) {
    // Verify that shortcuts were loaded from the config file
    EXPECT_TRUE(keyConfig->hasShortcut("test.command1"));
    EXPECT_TRUE(keyConfig->hasShortcut("test.command2"));
    EXPECT_TRUE(keyConfig->hasShortcut("test.command3"));
}

TEST_F(KeyConfigTest, GetShortcut_ExistingCommand_ReturnsCorrectShortcut) {
    // Test getting shortcuts for existing commands
    const ShortcutConfig& shortcut1 = keyConfig->getShortcut("test.command1");
    EXPECT_EQ(shortcut1.modifier, "Ctrl");
    EXPECT_EQ(shortcut1.key, "F1");
    
    const ShortcutConfig& shortcut2 = keyConfig->getShortcut("test.command2");
    EXPECT_EQ(shortcut2.modifier, "Alt");
    EXPECT_EQ(shortcut2.key, "A");
    
    const ShortcutConfig& shortcut3 = keyConfig->getShortcut("test.command3");
    EXPECT_EQ(shortcut3.modifier, "Shift");
    EXPECT_EQ(shortcut3.key, "Space");
}

TEST_F(KeyConfigTest, GetShortcut_NonExistentCommand_ThrowsException) {
    // Test getting a shortcut for a non-existent command
    EXPECT_THROW(keyConfig->getShortcut("non.existent.command"), std::runtime_error);
}

TEST_F(KeyConfigTest, HasShortcut_ExistingCommand_ReturnsTrue) {
    // Test checking if shortcuts exist for existing commands
    EXPECT_TRUE(keyConfig->hasShortcut("test.command1"));
    EXPECT_TRUE(keyConfig->hasShortcut("test.command2"));
    EXPECT_TRUE(keyConfig->hasShortcut("test.command3"));
}

TEST_F(KeyConfigTest, HasShortcut_NonExistentCommand_ReturnsFalse) {
    // Test checking if a shortcut exists for a non-existent command
    EXPECT_FALSE(keyConfig->hasShortcut("non.existent.command"));
}

TEST_F(KeyConfigTest, GetConfiguredCommands_ReturnsAllCommands) {
    // Test getting all configured commands
    std::vector<std::string> commands = keyConfig->getConfiguredCommands();
    
    // Verify that all commands are returned
    EXPECT_THAT(commands, UnorderedElementsAre("test.command1", "test.command2", "test.command3"));
}

TEST_F(KeyConfigTest, Constructor_InvalidConfigFile_ThrowsException) {
    // Test with a non-existent config file
    fs::path nonExistentPath = fs::temp_directory_path() / "non_existent_config.ini";
    
    // Ensure the file doesn't exist
    if (fs::exists(nonExistentPath)) {
        fs::remove(nonExistentPath);
    }
    
    // Expect an exception when creating KeyConfig with a non-existent file
    EXPECT_THROW(KeyConfig invalidConfig(nonExistentPath.string()), std::runtime_error);
} 