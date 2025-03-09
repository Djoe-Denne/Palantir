#pragma once

#include "window/component/message/message_strategy.hpp"
#include "core_export.hpp"

namespace palantir::window::component::message {

/**
 * LoggerStrategy - A concrete implementation of MessageStrategy 
 * that logs message contents using the DEBUG_LOG utility.
 */
class PALANTIR_CORE_API LoggerStrategy : public MessageStrategy {
public:
    LoggerStrategy() = delete;
    ~LoggerStrategy() override = default;

    LoggerStrategy(const LoggerStrategy&) = delete;
    auto operator=(const LoggerStrategy&) -> LoggerStrategy& = delete;
    LoggerStrategy(LoggerStrategy&&) = delete;
    auto operator=(LoggerStrategy&&) -> LoggerStrategy& = delete;

    /**
     * Constructor with the event type this strategy handles.
     * 
     * @param eventType The event type string.
     */
    explicit LoggerStrategy(const std::string& eventType);
    
    /**
     * Execute the strategy by logging the message content.
     * 
     * @param json The JSON message to log.
     */
    auto execute(const nlohmann::json& json) -> void override;
    
    /**
     * Get the event type this strategy handles.
     * 
     * @return The event type string.
     */
    auto getEventType() const -> const std::string& override;
    
private:
#pragma warning(push)
#pragma warning(disable: 4251)
    std::string eventType_;
#pragma warning(pop)
};

} // namespace palantir::window::component::message 