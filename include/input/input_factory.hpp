#ifndef INPUT_FACTORY_HPP
#define INPUT_FACTORY_HPP

#include <memory>

#include "input/iinput.hpp"
namespace interview_cheater::input {
class InputFactory {
public:
    InputFactory() = default;
    ~InputFactory() = default;

    // Delete copy operations
    InputFactory(const InputFactory&) = delete;
    auto operator=(const InputFactory&) -> InputFactory& = delete;

    // Define move operations
    InputFactory(InputFactory&&) noexcept = default;
    auto operator=(InputFactory&&) noexcept -> InputFactory& = default;

    [[nodiscard]] static auto createInput() -> std::unique_ptr<IInput>;
};
}  // namespace interview_cheater::input
#endif  // INPUT_FACTORY_HPP