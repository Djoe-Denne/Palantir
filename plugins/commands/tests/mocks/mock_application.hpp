#pragma once

#include <gmock/gmock.h>
#include "Application.hpp"

namespace palantir::test {

// Mock base class to ensure virtual methods
class ApplicationBase {
public:
    virtual ~ApplicationBase() = default;
    virtual void show() = 0;
    virtual void stop() = 0;
    virtual int run() = 0;
    virtual void quit() = 0;
    virtual signal::SignalManager& getSignalManager() = 0;
    virtual std::shared_ptr<window::WindowManager> getWindowManager() = 0;

    virtual void attachSignals() = 0;
};

// Mock Application class inheriting from both Application and our base with virtuals
class MockApplication : public Application, public ApplicationBase {
public:    
    // Constructor with config path
    explicit MockApplication(const std::string& configPath) : Application() {
    }

    ~MockApplication() override = default;

    MOCK_METHOD(void, show, (), (override));
    MOCK_METHOD(void, stop, (), (override));
    MOCK_METHOD(int, run, (), (override));
    MOCK_METHOD(void, quit, (), (override));
    MOCK_METHOD((signal::SignalManager&), getSignalManager, (), (override));
    MOCK_METHOD((std::shared_ptr<window::WindowManager>), getWindowManager, (), (override));
    MOCK_METHOD(void, attachSignals, (), (override));
};

} // namespace palantir::test 