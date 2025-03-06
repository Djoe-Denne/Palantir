#pragma once

#include "mock/palantir_mock.hpp"
#include "command/icommand.hpp"

namespace palantir::test {

class MockCommand : public command::ICommand, public PalantirMock {
public:
    MOCK_METHOD(void, execute, (), (override));
    MOCK_METHOD(bool, useDebounce, (), (override));
};

} // namespace palantir::test
