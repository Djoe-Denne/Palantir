#pragma once

#include "window/component/message/message_strategy_concept.hpp"
#include "mock/palantir_mock.hpp"

namespace palantir::test {

class MockMessageStrategy : public palantir::window::component::message::MessageStrategyBase, public PalantirMock {
public:
    MOCK_METHOD(const std::string&, getEventType, (), (const, override));
    MOCK_METHOD(void, executeJson, (const nlohmann::json&), (override));
};

} // namespace palantir::test
	