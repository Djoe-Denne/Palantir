#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "core_export.hpp"

namespace palantir::window::component::message {

/**
 * Abstract base class for handling different types of WebView messages
 * using the Strategy pattern.
 */
class PALANTIR_CORE_API MessageStrategy {
public:
    MessageStrategy() = default;
    virtual ~MessageStrategy() = default;

    MessageStrategy(const MessageStrategy&) = delete;
    auto operator=(const MessageStrategy&) -> MessageStrategy& = delete;
    MessageStrategy(MessageStrategy&&) = delete;
    auto operator=(MessageStrategy&&) -> MessageStrategy& = delete;

    /**
     * Execute the strategy with the provided JSON message.
     * 
     * @param json The JSON message to process.
     */
    virtual auto execute(const nlohmann::json& json) -> void = 0;

    /**
     * Get the event type that this strategy handles.
     * 
     * @return The event type string.
     */
    virtual auto getEventType() const -> const std::string& = 0;

protected:
};

} // namespace palantir::window::component::message 