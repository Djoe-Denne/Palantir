#pragma once

#include "mock/palantir_mock.hpp"
#include "input/input_factory.hpp"
#include "config/config.hpp"

namespace palantir::test {

class MockInputFactory : public input::InputFactory, public PalantirMock {
public:
    MockInputFactory(const std::shared_ptr<config::Config>& config) : InputFactory(config) {}
    ~MockInputFactory() override = default;

    MOCK_METHOD(void, initialize, (), (override));
    MOCK_METHOD(std::unique_ptr<input::IInput>, createInput, (const std::string& commandName), (const, override));
    MOCK_METHOD(bool, hasShortcut, (const std::string& commandName), (const, override));
    MOCK_METHOD(std::vector<std::string>, getConfiguredCommands, (), (const, override));
};

}  // namespace palantir::test