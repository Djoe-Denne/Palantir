#pragma once

#include "mock/palantir_mock.hpp"
#include "signal/signal_factory.hpp"


namespace palantir::test {

class MockSignalFactory : public signal::SignalFactory, public PalantirMock {
public:
    MockSignalFactory() = default;
    MockSignalFactory(const std::shared_ptr<input::InputFactory>& inputFactory) : SignalFactory(inputFactory) {}
    ~MockSignalFactory() override = default;

    MOCK_METHOD(std::vector<std::unique_ptr<signal::ISignal>>, createSignals, (), (const, override));
};

}  // namespace palantir::test