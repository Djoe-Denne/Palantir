#pragma once

#include "mock/palantir_mock.hpp"
#include "Application.hpp"

namespace palantir::test {

// Mock Application class inheriting from both Application and our base with virtuals
class MockApplication : public Application, public PalantirMock {
public:    
    // Constructor with config path
    explicit MockApplication(const std::string& configPath) : Application() {
    }

    ~MockApplication() override = default;

    MOCK_METHOD(int, run, (), (override));
    MOCK_METHOD(void, quit, (), (override));
    MOCK_METHOD((const std::shared_ptr<signal::ISignalManager>&), getSignalManager, (), (const, override));
    MOCK_METHOD((const std::shared_ptr<window::WindowManager>&), getWindowManager, (), (const, override));
    MOCK_METHOD(void, attachSignals, (), (override));
};

} // namespace palantir::test 