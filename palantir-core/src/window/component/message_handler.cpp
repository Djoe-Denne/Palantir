#include "window/component/message/message_handler.hpp"

#include <nlohmann/json.hpp>

#include "utils/logger.hpp"

namespace palantir::window::component::message {

auto MessageHandler::registerStrategy(std::unique_ptr<MessageStrategy> strategy) -> void {
    if (!strategy) {
        DEBUG_LOG("Attempted to register a null strategy");
        return;
    }
    
    std::string eventType = strategy->getEventType();
    strategies_.push_back(std::move(strategy));
    DEBUG_LOG("Registered strategy for event type: ", eventType);
}

auto MessageHandler::handleMessage(const std::string& message) -> void {
    try {
        nlohmann::json jsonMessage = nlohmann::json::parse(message);
        
        // Check if the message has a type field
        if (!jsonMessage.contains("type") || !jsonMessage["type"].is_string()) {
            DEBUG_LOG("Message is missing type field or type is not a string: ", message);
            return;
        }
        
        std::string eventType = jsonMessage["type"];
        
        for (const auto& strategy : strategies_) {
            if (strategy->getEventType() == "*" || strategy->getEventType() == eventType) {
                strategy->execute(jsonMessage);
            }
        }
        
    } 
    catch (const nlohmann::json::parse_error& e) {
        DEBUG_LOG("Failed to parse message as JSON: ", e.what(), " Message: ", message);
    }
    catch (const std::exception& e) {
        DEBUG_LOG("Exception in handleMessage: ", e.what());
    }
    catch (...) {
        DEBUG_LOG("Unknown exception in handleMessage");
    }
    
}

} // namespace palantir::window::component::message 