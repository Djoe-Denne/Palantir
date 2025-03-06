#pragma once

#include "mock/palantir_mock.hpp"
#include "input/key_register.hpp"

namespace palantir::test {

// Mock KeyConfig class inheriting from both KeyConfig and our base with virtuals
class MockKeyRegister : public input::KeyRegister, public PalantirMock {
public:    
    // Constructor with config path
    explicit MockKeyRegister() = default;
    ~MockKeyRegister() override = default;

    MOCK_METHOD(void, registerKey, (const std::string& key, int value), (override));
    MOCK_METHOD(int, get, (const std::string& key), (const, override));
    MOCK_METHOD(bool, hasKey, (const std::string& key), (const, override));
};

} // namespace palantir::test 