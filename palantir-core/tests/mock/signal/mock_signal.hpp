#pragma once

#include "mock/palantir_mock.hpp"
#include "signal/input_signal.hpp"

namespace palantir::test {

class MockSignal : public signal::ISignal, public PalantirMock {
public:
    ~MockSignal() override = default;

    MOCK_METHOD(void, start, (), (override));
    MOCK_METHOD(void, stop, (), (override));
    MOCK_METHOD(bool, isActive, (), (const, override));
    MOCK_METHOD(void, check, (const std::any&), (override));
};

}  // namespace palantir::test