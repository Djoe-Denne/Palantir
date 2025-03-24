/**
 * @file input_factory.hpp
 * @brief Defines the factory for creating input handlers.
 *
 * This file contains the InputFactory template class which is responsible for managing
 * concrete input factories that create input handlers based on configuration.
 */

#ifndef INPUT_FACTORY_HPP
#define INPUT_FACTORY_HPP

#include <filesystem>
#include <memory>
#include <string>
#include <vector>
#include <type_traits>
#include <tuple>

#include "core_export.hpp"
#include "input/keyboard_Input.hpp"
#include "input/key_config.hpp"
#include "input/key_mapper.hpp"

namespace palantir::input {

#if __cplusplus >= 202002L
template<typename CF>
concept ConcreteFactory = requires(CF cf) {
    { cf.initialize() } -> std::same_as<void>;
    { cf.createInput(std::string()) } -> std::same_as<std::unique_ptr<IInput>>;
    { cf.hasShortcut(std::string()) } -> std::same_as<bool>;
    { cf.getConfiguredCommands() } -> std::same_as<std::vector<std::string>>;
};
#endif
/**
 * @class InputFactory
 * @brief Templated factory class for managing concrete input handlers factories.
 *
 * This factory class is responsible for managing concrete input factories.
 * It provides a centralized way to access different input factory implementations.
 * @tparam ConcreteFactories A list of concrete factory classes that will be used to create inputs.
 */
#if __cplusplus >= 202002L
template<ConcreteFactory... ConcreteFactories>
#else
template<typename... ConcreteFactories>
#endif
class PALANTIR_CORE_API InputFactory {
    // Check that all types are valid and inherit from the expected base class
    static_assert(sizeof...(ConcreteFactories) > 0, "At least one concrete factory must be provided");

public:
    /**
     * @brief Constructor that initializes all concrete factories.
     */
    InputFactory() : factories_(std::make_tuple(std::make_unique<ConcreteFactories>()...)) {}

    /**
     * @brief Destructor.
     */
    ~InputFactory() = default;

    // Delete copy operations
    InputFactory(const InputFactory&) = delete;
    auto operator=(const InputFactory&) -> InputFactory& = delete;

    // Delete move operations
    InputFactory(InputFactory&&) = delete;
    auto operator=(InputFactory&&) -> InputFactory& = delete;

    /**
     * @brief Initialize all factories with the given configuration path.
     * @param configPath Path to the configuration file.
     *
     * This method must be called before using any other methods.
     * It calls the initialize method on all concrete factories.
     */
    auto initialize() -> void {
        std::apply([&](auto&... factoryPtrs) {
            (factoryPtrs->initialize(), ...);
        }, factories_);
    }

    /**
     * @brief Create an input handler for a specific command.
     * @param commandName Name of the command to create input for.
     * @return A unique pointer to the created input handler.
     *
     * Creates and returns an input handler configured for the specified command.
     * iterates over all factories and calls createInput on each one. Return as soon as a valid input is found.
     * 
     * @throws NoCommandFoundException if no command is found.
     */
    
[[nodiscard]] virtual auto createInput(const std::string& commandName) const -> std::unique_ptr<IInput> override {
    return tryCreateInput(commandName, std::make_index_sequence<sizeof...(ConcreteFactories)>{});
}

/**
 * @brief Check if a shortcut exists for a command.
 * @param commandName Name of the command to check.
 * @return true if a shortcut exists, false otherwise.
 */
[[nodiscard]] virtual auto hasShortcut(const std::string& commandName) const -> bool override {
    return tryHasShortcut(commandName, std::make_index_sequence<sizeof...(ConcreteFactories)>{});
}

/**
 * @brief Get all configured commands.
 * @return A vector of all configured commands.
 */
[[nodiscard]] virtual auto getConfiguredCommands() const -> std::vector<std::string> override {
    std::vector<std::string> commands;
    std::apply([&](const auto&... factoryPtrs) {
        (commands.insert(commands.end(),
                         factoryPtrs->getConfiguredCommands().begin(),
                         factoryPtrs->getConfiguredCommands().end()), ...);
    }, factories_);
    return commands;
}

private:

    template<std::size_t... Is>
    auto tryCreateInput(const std::string& commandName, std::index_sequence<Is...>) const -> std::unique_ptr<IInput> {
        std::unique_ptr<IInput> result;

        // fold expression over comma to short-circuit
        bool found = ((result = std::get<Is>(factories_)->hasShortcut(commandName)
                    ? std::get<Is>(factories_)->createInput(commandName)
                    : nullptr, result != nullptr) || ...);

        if (!found) {
            throw exception::TraceableNoCommandFoundException("No command found");
        }
        return result;
    }

    template<std::size_t... Is>
    auto tryHasShortcut(const std::string& commandName, std::index_sequence<Is...>) const -> bool {
        return ((std::get<Is>(factories_)->hasShortcut(commandName)) || ...);
    }

    // Tuple of unique pointers to all concrete factories
    std::tuple<std::unique_ptr<ConcreteFactories>...> factories_;
};

}  // namespace palantir::input

#endif  // INPUT_FACTORY_HPP