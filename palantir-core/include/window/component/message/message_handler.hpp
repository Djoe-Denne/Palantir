#pragma once

#include <vector>
#include <memory>
#include <string>
#include "window/component/message/message_strategy_concept.hpp"
#include "core_export.hpp"

namespace palantir::window::component::message {

/**
 * MessageHandler class that manages different message strategies
 * and routes messages to the appropriate strategy based on event type.
 */
class PALANTIR_CORE_API MessageHandler {
public:
    MessageHandler() = default;
    ~MessageHandler() = default;

    MessageHandler(const MessageHandler&) = delete;
    auto operator=(const MessageHandler&) -> MessageHandler& = delete;
    MessageHandler(MessageHandler&&) = delete;
    auto operator=(MessageHandler&&) -> MessageHandler& = delete;

    /**
     * Register a new message strategy.
     * 
     * @param strategy Shared pointer to a MessageStrategyBase implementation.
     */
    auto registerStrategy(std::unique_ptr<MessageStrategyBase> strategy) -> void;

    /**
     * Handle a message by routing it to the appropriate strategy.
     * If no strategy is found for the message type, the message is ignored.
     * 
     * @param message The JSON message to handle as a string.
     */
    auto handleMessage(const std::string& message) -> void;

private:
#pragma warning(push)
#pragma warning(disable: 4251)
    // Collection of strategies with different parameter types
    std::vector<std::unique_ptr<MessageStrategyBase>> strategies_;
#pragma warning(pop)

};

} // namespace palantir::window::component::message 