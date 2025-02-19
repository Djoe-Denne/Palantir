#include "input/input_factory.hpp"
#include "input/iinput.hpp"
#include "input/input.hpp"
#include <memory>

namespace interview_cheater::input {
auto InputFactory::create() -> std::unique_ptr<IInput> {
  return std::make_unique<interview_cheater::input::Input>();
}
} // namespace interview_cheater::input