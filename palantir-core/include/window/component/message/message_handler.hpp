#pragma once

#include <vector>
#include <memory>
#include <string>
#include "window/component/message/message_strategy.hpp"
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
     * @param strategy Shared pointer to a MessageStrategy implementation.
     */
    auto registerStrategy(std::unique_ptr<MessageStrategy> strategy) -> void;

    /**
     * Handle a message by routing it to the appropriate strategy.
     * If no strategy is found for the message type, the message is ignored.
     * 
     * @param messageJson The JSON message to handle.
     * @return True if the message was handled by a strategy, false otherwise.
     */
    auto handleMessage(const std::string& message) -> void;

private:
#pragma warning(push)
#pragma warning(disable: 4251)
    // Map of event types to their corresponding strategies
    std::vector<std::unique_ptr<MessageStrategy>> strategies_;
#pragma warning(pop)

};

} // namespace palantir::window::component::message 