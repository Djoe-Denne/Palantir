#include "input/input_factory.hpp"

#ifdef _WIN32
#include "platform/windows/input.hpp"
#elif defined(__APPLE__)
#include "platform/macos/input.hpp"
#endif

std::unique_ptr<IInput> InputFactory::create() {
    return std::make_unique<Input>();
} 