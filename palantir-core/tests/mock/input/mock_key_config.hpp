#pragma once

#include "mock/palantir_mock.hpp"
#include "input/key_config.hpp"

namespace palantir::test {

// Mock KeyConfig class inheriting from both KeyConfig and our base with virtuals
class MockKeyConfig : public input::KeyConfig, public PalantirMock {
public:    
    // Constructor with config path
    explicit MockKeyConfig(const std::string& configPath) 
        : input::KeyConfig(configPath) {
    }

    ~MockKeyConfig() override = default;

    MOCK_METHOD((const input::ShortcutConfig&), getShortcut, (const std::string& commandName), (const, override));
    MOCK_METHOD(bool, hasShortcut, (const std::string& commandName), (const, override));
    MOCK_METHOD((std::vector<std::string>), getConfiguredCommands, (), (const, override));
};

} // namespace palantir::test 