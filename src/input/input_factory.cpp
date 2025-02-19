#include "input/input_factory.hpp"
#include "input/input.hpp"

using namespace interview_cheater::input;

std::unique_ptr<IInput> InputFactory::create() {
    return std::make_unique<Input>();
} 