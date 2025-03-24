#pragma once

#if __has_include(<concepts>)
#include <concepts>
#include <type_traits>
#define PALANTIR_HAS_CONCEPTS
#endif

namespace palantir {
namespace input {
class IInput;
}
namespace rules {

#ifdef PALANTIR_HAS_CONCEPTS


// Checks that cf.createInput(std::string) returns std::unique_ptr<IInput>
template<typename CF>
concept HasCreateInput = requires(CF cf) {
    { cf.createInput(std::string{}) } -> std::convertible_to<std::unique_ptr<input::IInput>>;
};

// Checks that cf.hasShortcut(std::string) returns bool
template<typename CF>
concept HasHasShortcut = requires(CF cf) {
    { cf.hasShortcut(std::string{}) } -> std::same_as<bool>;
};

// Checks that cf.getConfiguredCommands() returns vector<string>
template<typename CF>
concept HasGetConfiguredCommands = requires(CF cf) {
    { cf.getConfiguredCommands() } -> std::same_as<std::vector<std::string>>;
};

template<typename CF>
concept InputConcreteFactory =
    HasCreateInput<CF> &&
    HasHasShortcut<CF> &&
    HasGetConfiguredCommands<CF>;

#define INPUT_FACTORY_TEMPLATE palantir::rules::InputConcreteFactory
#else
#define INPUT_FACTORY_TEMPLATE typename
#endif

} // namespace rules
} // namespace palantir
