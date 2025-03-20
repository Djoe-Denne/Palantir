#pragma once

#include "mock/palantir_mock.hpp"
#include "signal/signal_factory.hpp"
#include "signal/keyboard_input_signal_factory.hpp"

namespace palantir::test {

template<typename... ConcreteFactories>
class MockSignalFactory : public signal::SignalFactory<ConcreteFactories...>, public PalantirMock {
public:
    ~MockSignalFactory() override = default;

    MOCK_METHOD(std::vector<std::unique_ptr<signal::ISignal>>, createSignals, (), (const, override));
};

}  // namespace palantir::test