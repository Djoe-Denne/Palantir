#pragma once

#include "mock/palantir_mock.hpp"
#include "input/input_factory.hpp"


namespace palantir::test {

template<typename... Factories>
class MockInputFactory : public input::InputFactory<Factories...>, public PalantirMock {
public:
    ~MockInputFactory() override = default;

    MOCK_METHOD(std::unique_ptr<input::IInput>, createInput, (const std::string& commandName), (const, override));
    MOCK_METHOD(bool, hasShortcut, (const std::string& commandName), (const, override));
    MOCK_METHOD(std::vector<std::string>, getConfiguredCommands, (), (const, override));
};

}  // namespace palantir::test