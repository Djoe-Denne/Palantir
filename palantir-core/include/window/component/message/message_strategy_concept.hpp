#pragma once

#include <concepts>
#include <nlohmann/json.hpp>
#include <string>

#include "core_export.hpp"

namespace palantir::window::component::message {

/**
 * MessageStrategy concept that defines the requirements for a strategy class
 * This concept allows for different parameter types in the execute method
 */
template <typename T>
concept MessageStrategyConcept = requires(T strategy, const std::string& event_type, typename T::VOType vo) {
    // Require that the strategy has a getEventType method that returns a string
    { strategy.getEventType() } -> std::convertible_to<const std::string&>;
    { strategy.execute(vo) };
};

/**
 * Base strategy interface to allow type erasure for storing different strategy types
 * This acts as the non-template base for our type-erased wrapper
 */
class PALANTIR_CORE_API MessageStrategyBase {
public:
    MessageStrategyBase() = default;
    virtual ~MessageStrategyBase() = default;

    MessageStrategyBase(const MessageStrategyBase&) = delete;
    auto operator=(const MessageStrategyBase&) -> MessageStrategyBase& = delete;
    MessageStrategyBase(MessageStrategyBase&&) = delete;
    auto operator=(MessageStrategyBase&&) -> MessageStrategyBase& = delete;

    // All strategies must provide a way to identify their event type
    [[nodiscard]] virtual auto getEventType() const -> const std::string& = 0;

    // Common execute method that accepts JSON - will be implemented by the wrapper
    virtual auto executeJson(const nlohmann::json& json) -> void = 0;
};

/**
 * Type-erased wrapper for concrete strategy implementations
 * This allows storing different strategy types with different execute parameter types
 * in a common container while preserving type safety
 */
template <typename ConcreteStrategy>
    requires MessageStrategyConcept<ConcreteStrategy>
class TypeErasedStrategy : public MessageStrategyBase {
public:
    explicit TypeErasedStrategy(std::unique_ptr<ConcreteStrategy> strategy) : strategy_(std::move(strategy)) {}

    [[nodiscard]] auto getEventType() const -> const std::string& override { return strategy_->getEventType(); }

    auto executeJson(const nlohmann::json& json) -> void override {
        // Convert JSON to the specific parameter type and call the concrete strategy
        auto param = ConcreteStrategy::Mapper::fromJson(json);
        strategy_->execute(param);
    }

private:
    std::unique_ptr<ConcreteStrategy> strategy_;
};

// Helper function to create a type-erased strategy
template <typename ConcreteStrategy>
auto makeStrategy(std::unique_ptr<ConcreteStrategy> strategy) {
    return std::make_unique<TypeErasedStrategy<ConcreteStrategy>>(std::move(strategy));
}

}  // namespace palantir::window::component::message