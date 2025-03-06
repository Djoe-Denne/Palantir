#pragma once

#include "mock/palantir_mock.hpp"
#include "input/configurable_input.hpp"
#include "input/iinput.hpp"

namespace palantir::test {

// Mock ConfigurableInput class inheriting from both ConfigurableInput and our base with virtuals
class MockConfigurableInput : public input::ConfigurableInput, public PalantirMock {
public:    
    // Constructor with key code and modifier code
    explicit MockConfigurableInput(int keyCode, int modifierCode) 
        : input::ConfigurableInput(keyCode, modifierCode) {
    }

    ~MockConfigurableInput() override = default;

    MOCK_METHOD(bool, isKeyPressed, (const std::any& event), (const, override));
    MOCK_METHOD(bool, isModifierActive, (const std::any& event), (const, override));
    MOCK_METHOD(void, update, (), (override));
};

} // namespace palantir::test 