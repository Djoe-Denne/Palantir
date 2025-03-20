#pragma once

#include "mock/palantir_mock.hpp"
#include "input/keyboard_Input.hpp"
#include "input/iinput.hpp"

namespace palantir::test {

// Mock KeyboardInput class inheriting from both KeyboardInput and our base with virtuals
class MockKeyboardInput : public input::KeyboardInput, public PalantirMock {
public:    
    // Constructor with key code and modifier code
    explicit MockKeyboardInput(int keyCode, int modifierCode) 
        : input::KeyboardInput(keyCode, modifierCode) {
    }

    ~MockKeyboardInput() override = default;

    MOCK_METHOD(bool, isActive, (const std::any& event), (const, override));
    MOCK_METHOD(void, update, (), (override));
};

} // namespace palantir::test 