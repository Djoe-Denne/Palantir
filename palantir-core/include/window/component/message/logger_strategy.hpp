#pragma once

#include "window/component/message/log_message_vo.hpp"
#include "window/component/message/log_message_mapper.hpp"
#include "core_export.hpp"

namespace palantir::window::component::message {

/**
 * LoggerStrategy - A concrete strategy implementation that uses a strongly typed VO
 * rather than raw JSON.
 */
class PALANTIR_CORE_API LoggerStrategy {
public:
    using VOType = LogMessageVO;
    using Mapper = LogMessageMapper;
    LoggerStrategy() = delete;
    ~LoggerStrategy() = default;

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
     * Execute the strategy using the strongly typed value object
     * 
     * @param logMessage The typed log message value object
     */
    auto execute(const LogMessageVO& logMessage) -> void;
    
    /**
     * Get the event type this strategy handles.
     * 
     * @return The event type string.
     */
    auto getEventType() const -> const std::string&;
    
private:
#pragma warning(push)
#pragma warning(disable: 4251)
    std::string eventType_;
#pragma warning(pop)
};
} // namespace palantir::window::component::message 