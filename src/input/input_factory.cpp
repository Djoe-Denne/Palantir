#include "input/input_factory.hpp"
#include "input/configurable_input.hpp"
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <memory>
#include "input/key_mapper.hpp"
#include "utils/logger.hpp"

namespace interview_cheater::input {

std::unique_ptr<KeyConfig> InputFactory::keyConfig_;

auto InputFactory::initialize(const std::string& configPath) -> void {
    // Create directory if it doesn't exist
    const auto directory = std::filesystem::path(configPath).parent_path();
    if (!std::filesystem::exists(directory)) {
        std::filesystem::create_directories(directory);
    }

    // Create default config if file doesn't exist
    if (!std::filesystem::exists(configPath)) {
        createDefaultConfig(configPath);
    }

    keyConfig_ = std::make_unique<KeyConfig>(configPath);
}

auto InputFactory::createDefaultConfig(const std::string& configPath) -> void {
    std::ofstream configFile(configPath);
    if (!configFile) {
        throw std::runtime_error("Failed to create default config file: " + configPath);
    }

    // Write default configuration
    configFile << "; Default keyboard shortcuts configuration\n"
               << "; Format: command = modifier+key\n"
               << "; Available modifiers: Ctrl, Alt, Shift, Win (Windows) / Cmd (macOS)\n"
               << "\n"
               << "[commands]\n"
#ifdef _WIN32
               << "toggle = Ctrl+F1    ; Toggle window visibility\n"
               << "stop = Win+/        ; Stop application\n"
#else
               << "toggle = Ctrl+F1    ; Toggle window visibility\n"
               << "stop = Cmd+/        ; Stop application\n"
#endif
               << "\n";

    if (!configFile) {
        throw std::runtime_error("Failed to write default configuration to: " + configPath);
    }
}

/**
 * @brief Create a new input object from configuration.
 * @param config Reference to the key configuration data.
 * @return Unique pointer to the created input object.
 * @throws std::invalid_argument if the configuration is invalid.
 * 
 * Delegates to the implementation's createInput method to handle
 * input object creation and configuration.
 */
[[nodiscard]] auto InputFactory::createInput(const std::string& commandName) -> std::unique_ptr<ConfigurableInput> {
    if (!keyConfig_) {
        throw std::runtime_error("InputFactory not initialized. Call initialize() first.");
    }
    const auto& shortcut = keyConfig_->getShortcut(commandName);
    if (!KeyMapper::isValidKey(shortcut.key) || !KeyMapper::isValidModifier(shortcut.modifier)) {
        throw std::invalid_argument("Invalid shortcut configuration for command: " + commandName);
    }
    int keyCode = KeyMapper::getKeyCode(shortcut.key);
    int modifierCode = KeyMapper::getModifierCode(shortcut.modifier);
    return std::make_unique<ConfigurableInput>(keyCode, modifierCode);
}

auto InputFactory::hasShortcut(const std::string& commandName) -> bool {
    if (!keyConfig_) {
        throw std::runtime_error("InputFactory not initialized. Call initialize() first.");
    }
    return keyConfig_->hasShortcut(commandName);
}

auto InputFactory::getConfiguredCommands() -> std::vector<std::string> {
    if (!keyConfig_) {
        throw std::runtime_error("InputFactory not initialized. Call initialize() first.");
    }
    return keyConfig_->getConfiguredCommands();
}

}  // namespace interview_cheater::input 