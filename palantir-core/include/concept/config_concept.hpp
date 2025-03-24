#pragma once

#include <filesystem>
#if __has_include(<concepts>)
#include <concepts>
#include <type_traits>
#define PALANTIR_HAS_CONCEPTS
#endif

#include "concept/input_concept.hpp"

namespace palantir {
namespace input {
template<INPUT_FACTORY_TEMPLATE... Factories>
class InputFactory;
}
namespace rules {

#ifdef PALANTIR_HAS_CONCEPTS

// Check if type has method getShortcutsPath() returning something convertible to std::filesystem::path
template<typename T>
concept HasGetShortcutsPath = requires(T t) {
    { t.getShortcutsPath() } -> std::convertible_to<std::filesystem::path>;
};

// Check if type has nested alias DefaultInputFactory
template<typename T>
concept HasDefaultInputFactoryAlias = requires {
    typename T::DefaultInputFactory;
};

// Check if T::DefaultInputFactory is an input::InputFactory<...>
template<typename T>
struct is_input_factory : std::false_type {};

template<typename... Factories>
struct is_input_factory<input::InputFactory<Factories...>> : std::true_type {};

template<typename T>
concept HasValidFactoryAliasType = HasDefaultInputFactoryAlias<T> &&
                                   is_input_factory<typename T::DefaultInputFactory>::value;

template<typename T>
concept HasConfigRequirements =
    HasGetShortcutsPath<T> &&
    HasDefaultInputFactoryAlias<T> &&
    HasValidFactoryAliasType<T>;

#define CONFIG_TYPE ConfigT
#define CONFIG_CHECKS template<palantir::rules::HasConfigRequirements ConfigT>
#else
#define CONFIG_TYPE palantir::config::Config
#define CONFIG_CHECKS
#endif
} // namespace rules
} // namespace palantir