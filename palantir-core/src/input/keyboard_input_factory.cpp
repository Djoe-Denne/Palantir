#include "input/keyboard_input_factory.hpp"

#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>

#include "config/config.hpp"
#include "exception/exceptions.hpp"
#include "input/key_mapper.hpp"
#include "input/key_config.hpp"
#include "input/keyboard_Input.hpp"
#include "utils/logger.hpp"

namespace palantir::input {

class KeyboardInputFactory::KeyboardInputFactoryImpl {
public:
    KeyboardInputFactoryImpl(const std::shared_ptr<config::Config>& config) : config_(config) {}
    ~KeyboardInputFactoryImpl() = default;

    KeyboardInputFactoryImpl(const KeyboardInputFactoryImpl&) = delete;
    auto operator=(const KeyboardInputFactoryImpl&) -> KeyboardInputFactoryImpl& = delete;
    KeyboardInputFactoryImpl(KeyboardInputFactoryImpl&&) = delete;
    auto operator=(KeyboardInputFactoryImpl&&) -> KeyboardInputFactoryImpl& = delete;

    auto initialize() -> void {
        auto configPath = config_->getConfigPath() / ("shortcuts." + config_->getConfigurationFormat());
        // Create directory if it doesn't exist
        if (!std::filesystem::exists(configPath.parent_path())) {
            std::filesystem::create_directories(configPath.parent_path());
        }

        // Create default config if file doesn't exist
        if (!std::filesystem::exists(configPath)) {
            createDefaultConfig(configPath);
        }

        keyConfig_ = std::make_unique<KeyConfig>(configPath);
    }

    /**
     * @brief Create a default configuration file.
     * @param configPath Path where the default configuration should be created.
     *
     * Creates a default configuration file with platform-specific shortcuts
     * when no configuration file exists.
     * @throws std::runtime_error if the file cannot be created or written to.
     */
    auto createDefaultConfig(const std::filesystem::path& configPath) const -> void {
        std::ofstream configFile(configPath);
        if (!configFile) {
            throw palantir::exception::TraceableConfigFileException("Failed to create default config file: " +
                                                                    configPath.string());
        }

        // Write default configuration
        configFile
            << "; Default keyboard shortcuts configuration\n"
            << "; Format: command = modifier+key\n"
            << "; Available modifiers: Ctrl, Alt, Shift, Win (Windows) / Cmd (macOS)\n"
            << "\n"
            << "[commands]\n"
#ifdef _WIN32
            << "toggle = Ctrl+Num 1    ; Toggle window visibility\n"
            << "stop = Win+/        ; Stop application\n"
            << "window-screenshot = Ctrl+Num 2    ; Take screenshot of current window\n"
            << "toggle-transparency = Ctrl+Num 3    ; Toggle window transparency\n"
            << "toggle-window-anonymity = Ctrl+Num 4    ; Toggle window anonymity\n"
            << "send-sauron-implement-request = Ctrl+Num 4    ; Send request to Sauron to implement\n"
            << "send-sauron-fix-errors-request = Ctrl+Num 5    ; Send request to Sauron to fix errors\n"
            << "send-sauron-validate-with-tests-request = Ctrl+Num 6    ; Send request to Sauron to validate with "
               "tests\n"
            << "send-sauron-fix-test-failures-request = Ctrl+Num 7    ; Send request to Sauron to fix test failures\n"
            << "send-sauron-handle-todos-request = Ctrl+Num 8    ; Send request to Sauron to handle todos\n"
            << "clear-screenshot = Ctrl+Num 9    ; Clear screenshot folder\n"
#else
            << "toggle = Ctrl+F1    ; Toggle window visibility\n"
            << "stop = Cmd+/        ; Stop application\n"
#endif
            << "\n";

        if (!configFile) {
            throw palantir::exception::TraceableConfigFileException("Failed to write default configuration to: " +
                                                                    configPath.string());
        }
    }

    [[nodiscard]] auto createInput(const std::string& commandName) const -> std::unique_ptr<IInput> {
        if (!keyConfig_) {
            throw palantir::exception::TraceableInputFactoryException(
                "InputFactory not initialized. Call initialize() first.");
        }
        const auto& shortcut = keyConfig_->getShortcut(commandName);
        if (!KeyMapper::isValidKey(shortcut.key) || !KeyMapper::isValidModifier(shortcut.modifier)) {
            throw std::invalid_argument("Invalid shortcut configuration for command: " + commandName);
        }
        int keyCode = KeyMapper::getKeyCode(shortcut.key);
        int modifierCode = KeyMapper::getModifierCode(shortcut.modifier);
        return std::make_unique<KeyboardInput>(keyCode, modifierCode);
    }

    [[nodiscard]] auto hasShortcut(const std::string& commandName) const -> bool {
        if (!keyConfig_) {
            throw palantir::exception::TraceableInputFactoryException(
                "InputFactory not initialized. Call initialize() first.");
        }
        return keyConfig_->hasShortcut(commandName);
    }

    [[nodiscard]] auto getConfiguredCommands() const -> std::vector<std::string> {
        if (!keyConfig_) {
            throw palantir::exception::TraceableInputFactoryException(
                "InputFactory not initialized. Call initialize() first.");
        }
        return keyConfig_->getConfiguredCommands();
    }

private:
    std::unique_ptr<KeyConfig> keyConfig_;
    std::shared_ptr<config::Config> config_;
};

KeyboardInputFactory::KeyboardInputFactory(const std::shared_ptr<config::Config>& config)
    : pimpl_(std::make_unique<KeyboardInputFactoryImpl>(config)) {}
KeyboardInputFactory::~KeyboardInputFactory() = default;

auto KeyboardInputFactory::initialize() -> void { pimpl_->initialize(); }

/**
 * @brief Create a new input object from configuration.
 * @param config Reference to the key configuration data.
 * @return Unique pointer to the created input object.
 * @throws std::invalid_argument if the configuration is invalid.
 *
 * Delegates to the implementation's createInput method to handle
 * input object creation and configuration.
 */
[[nodiscard]] auto KeyboardInputFactory::createInput(const std::string& commandName) const -> std::unique_ptr<IInput> {
    return pimpl_->createInput(commandName);
}

auto KeyboardInputFactory::hasShortcut(const std::string& commandName) const -> bool {
    return pimpl_->hasShortcut(commandName);
}

auto KeyboardInputFactory::getConfiguredCommands() const -> std::vector<std::string> {
    return pimpl_->getConfiguredCommands();
}

}  // namespace palantir::input