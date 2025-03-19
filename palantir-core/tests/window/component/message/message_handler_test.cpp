#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <nlohmann/json.hpp>

#include "window/component/message/message_handler.hpp"
#include "window/component/message/resize/resize_strategy.hpp"
#include "window/component/message/resize/resize_message_vo.hpp"
#include "mock/window/component/mock_content_manager.hpp"
#include "mock/window/component/message/mock_message_strategy.hpp"

using namespace palantir::window::component::message;
using namespace palantir::window::component::message::resize;
using namespace palantir::test;
using namespace testing;

class MessageHandlerTest : public Test {
protected:
    void SetUp() override {
        handler = std::make_unique<MessageHandler>();
        mockStrategy = std::make_unique<MockMessageStrategy>();
    }

    void TearDown() override {
        handler.reset();
        mockStrategy.reset();
    }

    std::unique_ptr<MessageHandler> handler;
    std::unique_ptr<MockMessageStrategy> mockStrategy;
    const std::string eventType = "test";
};

TEST_F(MessageHandlerTest, RegisterStrategy_ValidStrategy_StrategyRegistered) {
    EXPECT_CALL(*mockStrategy, getEventType())
        .WillRepeatedly(ReturnRef(eventType));
    EXPECT_CALL(*mockStrategy, executeJson(_))
        .Times(1);

    handler->registerStrategy(std::move(mockStrategy));

    nlohmann::json message = {
        {"type", "test"},
        {"event", {}}
    };
    handler->handleMessage(message.dump());
}

TEST_F(MessageHandlerTest, HandleMessage_InvalidJson_NoStrategyExecuted) {
    EXPECT_CALL(*mockStrategy, getEventType())
        .WillOnce(ReturnRef(eventType));
    EXPECT_CALL(*mockStrategy, executeJson(_))
        .Times(0);

    handler->registerStrategy(std::move(mockStrategy));
    handler->handleMessage("invalid json");
}

TEST_F(MessageHandlerTest, HandleMessage_MissingType_NoStrategyExecuted) {
    EXPECT_CALL(*mockStrategy, getEventType())
        .WillOnce(ReturnRef(eventType));
    EXPECT_CALL(*mockStrategy, executeJson(_))
        .Times(0);

    handler->registerStrategy(std::move(mockStrategy));
    
    nlohmann::json message = {
        {"event", {}}
    };
    handler->handleMessage(message.dump());
}

TEST_F(MessageHandlerTest, HandleMessage_NonStringType_NoStrategyExecuted) {
    EXPECT_CALL(*mockStrategy, getEventType())
        .WillOnce(ReturnRef(eventType));
    EXPECT_CALL(*mockStrategy, executeJson(_))
        .Times(0);

    handler->registerStrategy(std::move(mockStrategy));
    
    nlohmann::json message = {
        {"type", 123},  // Non-string type
        {"event", {}}
    };
    handler->handleMessage(message.dump());
}

TEST_F(MessageHandlerTest, HandleMessage_WildcardStrategy_ExecutedForAllTypes) {
    const std::string eventType1 = "*";
    EXPECT_CALL(*mockStrategy, getEventType())
        .WillRepeatedly(ReturnRef(eventType1));
    EXPECT_CALL(*mockStrategy, executeJson(_))
        .Times(2);

    handler->registerStrategy(std::move(mockStrategy));

    nlohmann::json message1 = {
        {"type", "test1"},
        {"event", {}}
    };
    nlohmann::json message2 = {
        {"type", "test2"},
        {"event", {}}
    };
    
    handler->handleMessage(message1.dump());
    handler->handleMessage(message2.dump());
}

TEST_F(MessageHandlerTest, HandleMessage_MultipleStrategies_CorrectStrategyExecuted) {
    auto mockStrategy2 = std::make_unique<MockMessageStrategy>();
    const std::string eventType2 = "test2";
    EXPECT_CALL(*mockStrategy, getEventType())
        .WillRepeatedly(ReturnRef(eventType));
    EXPECT_CALL(*mockStrategy2, getEventType())
        .WillRepeatedly(ReturnRef(eventType2));
    
    EXPECT_CALL(*mockStrategy, executeJson(_))
        .Times(1);
    EXPECT_CALL(*mockStrategy2, executeJson(_))
        .Times(0);

    handler->registerStrategy(std::move(mockStrategy));
    handler->registerStrategy(std::move(mockStrategy2));

    nlohmann::json message = {
        {"type", "test"},
        {"event", {}}
    };
    handler->handleMessage(message.dump());
}

TEST_F(MessageHandlerTest, RegisterStrategy_NullStrategy_NoRegistration) {
    handler->registerStrategy(nullptr);
    // Test passes if no crash occurs
    
    nlohmann::json message = {
        {"type", "test"},
        {"event", {}}
    };
    handler->handleMessage(message.dump());
} 