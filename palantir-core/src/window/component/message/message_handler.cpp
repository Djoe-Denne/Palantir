#include "window/component/message/message_handler.hpp"

#include <nlohmann/json.hpp>

#include "utils/logger.hpp"

namespace palantir::window::component::message {

auto MessageHandler::registerStrategy(std::unique_ptr<MessageStrategyBase> strategy) -> void {
    if (!strategy) {
        DebugLog("Attempted to register a null strategy");
        return;
    }

    std::string eventType = strategy->getEventType();
    strategies_.push_back(std::move(strategy));
    DebugLog("Registered strategy for event type: ", eventType);
}

auto MessageHandler::handleMessage(const std::string& message) -> void {
    try {
        nlohmann::json jsonMessage = nlohmann::json::parse(message);

        // Check if the message has a type field
        if (!jsonMessage.contains("type") || !jsonMessage["type"].is_string()) {
            DebugLog("Message is missing type field or type is not a string: ", message);
            return;
        }

        std::string eventType = jsonMessage["type"];

        for (const auto& strategy : strategies_) {
            if (strategy->getEventType() == "*" || strategy->getEventType() == eventType) {
                // Call the type-erased executeJson method which will convert and forward to the typed execute
                strategy->executeJson(jsonMessage["event"]);
            }
        }

    } catch (const nlohmann::json::parse_error& e) {
        DebugLog("Failed to parse message as JSON: ", e.what(), " Message: ", message);
    } catch (const std::exception& e) {
        DebugLog("Exception in handleMessage: ", e.what());
    } catch (...) {
        DebugLog("Unknown exception in handleMessage");
    }
}

}  // namespace palantir::window::component::message