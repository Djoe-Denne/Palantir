#pragma once

#include "mock/palantir_mock.hpp"
#include "input/input_factory.hpp"


namespace palantir::test {

class MockInputFactory : public input::InputFactory, public PalantirMock {
public:
    ~MockInputFactory() override = default;

    MOCK_METHOD(void, initialize, (const std::string& configPath), (override));
    MOCK_METHOD(std::unique_ptr<input::ConfigurableInput>, createInput, (const std::string& commandName), (override));
    MOCK_METHOD(bool, hasShortcut, (const std::string& commandName), (const, override));
    MOCK_METHOD(std::vector<std::string>, getConfiguredCommands, (), (const, override));
};

}  // namespace palantir::test