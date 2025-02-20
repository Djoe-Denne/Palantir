#include "input/input_factory.hpp"

#ifdef _WIN32
#include "platform/windows/input/input.hpp"
#elif defined(__APPLE__)
#include "platform/macos/input/input.hpp"
#endif

namespace interview_cheater::input {

auto InputFactory::createInput() -> std::unique_ptr<IInput> {
    return std::make_unique<Input>();
}

}  // namespace interview_cheater::input