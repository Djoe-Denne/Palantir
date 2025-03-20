#pragma once

#include "mock/palantir_mock.hpp"
#include "input/input_factory.hpp"
#include "input/keyboard_input_factory.hpp"

namespace palantir::test {

template<typename... ConcreteFactories>
class MockInputFactory : public input::InputFactory<ConcreteFactories...>, public PalantirMock {
public:
    ~MockInputFactory() override = default;

    MOCK_METHOD(void, initialize, (const std::filesystem::path&), (override));
    MOCK_METHOD(std::unique_ptr<input::IInput>, createInput, (const std::string&), (const, override));
    MOCK_METHOD(bool, hasShortcut, (const std::string&), (const, override));
    MOCK_METHOD(std::vector<std::string>, getConfiguredCommands, (), (const, override));
};

}  // namespace palantir::test