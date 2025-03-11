# C++20 Features and Message Strategy Pattern

## C++20 Requirement

The project now requires a C++20 compatible compiler. This change was made to leverage several C++20 features that improve code quality and maintainability:

- **Concepts**: Used in the new message strategy pattern implementation
- **Type constraints**: Enhanced type safety in template code
- **Other C++20 features**: As needed throughout the codebase

### Build Configuration

The CMake configuration has been updated to require C++20:

```cmake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

### Compiler Requirements

- **Windows**: Visual Studio 2019 or higher with C++ workload
- **macOS**: Recent versions of Clang (Xcode 12+)
- **Linux**: GCC 10+ or Clang 10+

## Message Strategy Pattern Improvements

The message handling system has been refactored to use C++20 concepts for improved type safety and flexibility.

### Key Changes

1. Replaced the abstract base class approach with a concept-based strategy pattern
2. Added type-erasure to allow storage of different strategy types with varying parameter types
3. Introduced strongly-typed value objects for message data instead of raw JSON

### New Components

#### MessageStrategyConcept

```cpp
template <typename T>
concept MessageStrategyConcept = requires(T strategy, const std::string& event_type) {
    { strategy.getEventType() } -> std::convertible_to<const std::string&>;
    { strategy.execute(std::declval<typename T::VOType>()) };
};
```

This concept defines the requirements for any message strategy implementation.

#### MessageStrategyBase

A non-template base class that enables type erasure for storing different strategy types.

#### TypeErasedStrategy

```cpp
template <typename ConcreteStrategy>
requires MessageStrategyConcept<ConcreteStrategy>
class TypeErasedStrategy : public MessageStrategyBase {
    // Implementation details...
};
```

A template wrapper that adapts concrete strategy implementations to the common base interface.

#### Value Objects

Each strategy now uses a strongly-typed value object (VO) for its parameters instead of raw JSON:

```cpp
struct LogMessageVO {
    std::string message;
};
```

#### Mappers

Mapper classes handle conversion from JSON to the appropriate value object:

```cpp
class LogMessageMapper {
public:
    inline static auto fromJson(const nlohmann::json& json) -> LogMessageVO {
        return LogMessageVO{json.dump()};
    }
};
```

### Usage Example

Before:
```cpp
registerMessageStrategy(std::make_unique<message::LoggerStrategy>("*"));
```

After:
```cpp
registerMessageStrategy(message::makeStrategy(std::make_unique<message::LoggerStrategy>("*")));
```

### Benefits

- **Type safety**: Each strategy knows exactly what data type it needs
- **Improved maintainability**: Adding new strategy types doesn't require changing the base interface
- **Better error handling**: Type errors are caught at compile time rather than runtime
- **Clearer intent**: The code better expresses what data each strategy requires 