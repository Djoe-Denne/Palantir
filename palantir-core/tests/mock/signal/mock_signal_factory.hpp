#pragma once

#include "mock/palantir_mock.hpp"
#include "signal/isignal_factory.hpp"


namespace palantir::test {

class MockSignalFactory : public signal::ISignalFactory, public PalantirMock {
public:
    MockSignalFactory() = default;
    ~MockSignalFactory() override = default;

    MOCK_METHOD(std::vector<std::unique_ptr<signal::ISignal>>, createSignals, (), (const, override));
};

}  // namespace palantir::test