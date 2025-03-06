#pragma once

#include "mock/palantir_mock.hpp"
#include "command/command_factory.hpp"

namespace palantir::test {

class MockCommandFactory : public command::CommandFactory, public PalantirMock {
public:
    ~MockCommandFactory() override = default;

    MOCK_METHOD(void, registerCommand, (const std::string& commandName, command::CommandFactory::CommandCreator creator), (override));
    MOCK_METHOD(bool, unregisterCommand, (const std::string& commandName), (override));
    MOCK_METHOD(std::unique_ptr<command::ICommand>, getCommand, (const std::string& name), (override));
};

} // namespace palantir::test
