#include "window/component/message/logger/logger_strategy.hpp"

#include "utils/logger.hpp"

namespace palantir::window::component::message::logger {

LoggerStrategy::LoggerStrategy(std::string eventType) : eventType_(std::move(eventType)) {}

auto LoggerStrategy::execute(const LogMessageVO& logMessage) -> void {
    DEBUG_LOG("LoggerStrategy handling event: ", eventType_, " with message: ", logMessage.message);
}

auto LoggerStrategy::getEventType() const -> const std::string& { return eventType_; }

}  // namespace palantir::window::component::message::logger