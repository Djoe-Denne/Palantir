#pragma once

#include "mock/palantir_mock.hpp"
#include "command/icommand.hpp"

namespace palantir::test {

class MockCommand : public command::ICommand, public PalantirMock {
public:
    MOCK_METHOD(void, execute, (), (const, override));
    MOCK_METHOD(bool, useDebounce, (), (const, override));
};

} // namespace palantir::test
