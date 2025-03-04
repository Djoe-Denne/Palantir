#include "input/key_config.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "utils/logger.hpp"

namespace palantir::input {

KeyConfig::KeyConfig(const std::string& configPath) { loadConfig(configPath); }

auto KeyConfig::loadConfig(const std::string& configPath) -> void {
    std::ifstream configFile(configPath);
    if (!configFile) {
        throw std::runtime_error("Failed to open config file: " + configPath);
    }

    std::string line;
    bool inCommandSection = false;

    while (std::getline(configFile, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == ';') {
            continue;
        }

        // Check for section header
        if (line == "[commands]") {
            inCommandSection = true;
            continue;
        }

        if (!inCommandSection) {
            continue;
        }

        // Parse command = shortcut line
        auto equalPos = line.find('=');
        if (equalPos == std::string::npos) {
            continue;
        }

        std::string command = line.substr(0, equalPos);
        std::string shortcut = line.substr(equalPos + 1);

        // Trim whitespace
        command.erase(0, command.find_first_not_of(" \t"));
        command.erase(command.find_last_not_of(" \t") + 1);
        shortcut.erase(0, shortcut.find_first_not_of(" \t"));
        shortcut.erase(shortcut.find_last_not_of(" \t") + 1);

        // Remove comment if present
        auto commentPos = shortcut.find(';');
        if (commentPos != std::string::npos) {
            shortcut.erase(commentPos);
            shortcut.erase(shortcut.find_last_not_of(" \t") + 1);
        }

        // Parse modifier+key format
        auto plusPos = shortcut.find('+');
        if (plusPos == std::string::npos) {
            throw std::runtime_error("Invalid shortcut format for command: " + command);
        }

        ShortcutConfig config;
        config.modifier = shortcut.substr(0, plusPos);
        config.key = shortcut.substr(plusPos + 1);

        // Trim whitespace from modifier and key
        config.modifier.erase(0, config.modifier.find_first_not_of(" \t"));
        config.modifier.erase(config.modifier.find_last_not_of(" \t") + 1);
        config.key.erase(0, config.key.find_first_not_of(" \t"));
        config.key.erase(config.key.find_last_not_of(" \t") + 1);

        DEBUG_LOG("Loaded shortcut for {}: {}+{}", command, config.modifier, config.key);
        shortcuts_[command] = std::move(config);
    }
}

auto KeyConfig::getShortcut(const std::string& commandName) const -> const ShortcutConfig& {
    auto iterator = shortcuts_.find(commandName);
    if (iterator == shortcuts_.end()) {
        throw std::runtime_error("No shortcut configured for command: " + commandName);
    }
    return iterator->second;
}

auto KeyConfig::hasShortcut(const std::string& commandName) const -> bool {
    return shortcuts_.find(commandName) != shortcuts_.end();
}

auto KeyConfig::getConfiguredCommands() const -> std::vector<std::string> {
    std::vector<std::string> commands;
    commands.reserve(shortcuts_.size());
    for (const auto& [command, _] : shortcuts_) {
        commands.push_back(command);
    }
    return commands;
}

}  // namespace palantir::input