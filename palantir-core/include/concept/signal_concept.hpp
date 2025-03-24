#pragma once

#if __has_include(<concepts>)
#include <concepts>
#include <type_traits>
#include <memory>
#include <vector>
#define PALANTIR_HAS_CONCEPTS
#endif

namespace palantir {
namespace signal {
class ISignal;
}
namespace rules {

#ifdef PALANTIR_HAS_CONCEPTS

// Checks that sf.createSignals() returns vector<unique_ptr<ISignal>>
template<typename SF>
concept HasCreateSignals = requires(SF sf) {
    { sf.createSignals() } -> std::same_as<std::vector<std::unique_ptr<signal::ISignal>>>;
};

// Base concept for a signal factory
template<typename SF>
concept SignalConcreteFactory = HasCreateSignals<SF>;

#define SIGNAL_FACTORY_TEMPLATE palantir::rules::SignalConcreteFactory
#else
#define SIGNAL_FACTORY_TEMPLATE typename
#endif

} // namespace rules
} // namespace palantir 