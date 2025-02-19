#ifndef INPUT_FACTORY_HPP
#define INPUT_FACTORY_HPP

#include "input/iinput.hpp"

#include <memory>
namespace interview_cheater::input
{
class InputFactory
{
public:
    static std::unique_ptr<IInput> create();

private:
    InputFactory() = delete;
};
}  // namespace interview_cheater::input
#endif  // INPUT_FACTORY_HPP