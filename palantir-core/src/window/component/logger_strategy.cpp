#include "window/component/message/logger_strategy.hpp"
#include "utils/logger.hpp"

namespace palantir::window::component::message {

LoggerStrategy::LoggerStrategy(const std::string& eventType)
    : eventType_(eventType) {
}

auto LoggerStrategy::execute(const nlohmann::json& json) -> void {
    DEBUG_LOG("LoggerStrategy handling event: ", eventType_);
    DEBUG_LOG("Message content: ", json.dump(2));  // Pretty print with 2-space indentation
}

auto LoggerStrategy::getEventType() const -> const std::string& {
    return eventType_;
}

} // namespace palantir::window::component::message 