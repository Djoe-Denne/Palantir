#ifndef INPUT_FACTORY_HPP
#define INPUT_FACTORY_HPP

#include <memory>
#include "input/iinput.hpp"

class InputFactory {
public:
    static std::unique_ptr<IInput> create();
    
private:
    InputFactory() = delete;
};

#endif // INPUT_FACTORY_HPP 