#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <any>

#include "signal/keyboard_signal_manager.hpp"
#include "signal/isignal.hpp"
#include "mock/signal/mock_signal.hpp"
#include "mock/signal/mock_signal_factory.hpp"

using namespace palantir::signal;
using namespace palantir::test;
using namespace testing;

class KeyboardSignalManagerTest : public Test {
protected:
    void SetUp() override {
        mockFactory = std::make_shared<MockSignalFactory>();
        manager = std::make_shared<KeyboardSignalManager>(mockFactory);
    }

    void TearDown() override {
    }

    std::shared_ptr<MockSignalFactory> mockFactory;
    std::shared_ptr<KeyboardSignalManager> manager;
    std::any emptyEvent;
};

TEST_F(KeyboardSignalManagerTest, StartSignals_CreatesSignalsFromFactory) {
    auto mockSignal = std::make_unique<MockSignal>();
    auto* mockSignalPtr = mockSignal.get();
    
    std::vector<std::unique_ptr<ISignal>> signals;
    signals.push_back(std::move(mockSignal));
    
    EXPECT_CALL(*mockFactory, createSignals())
        .WillOnce(Return(ByMove(std::move(signals))));
    
    EXPECT_CALL(*mockSignalPtr, start()).Times(1);
    EXPECT_CALL(*mockSignalPtr, check(_)).Times(1);
    
    manager->startSignals();
    manager->checkSignals(emptyEvent);
}

TEST_F(KeyboardSignalManagerTest, StartSignals_CallsStartOnAllSignals) {
    auto mockSignal1 = std::make_unique<MockSignal>();
    auto mockSignal2 = std::make_unique<MockSignal>();
    auto* mockSignalPtr1 = mockSignal1.get();
    auto* mockSignalPtr2 = mockSignal2.get();
    
    std::vector<std::unique_ptr<ISignal>> signals;
    signals.push_back(std::move(mockSignal1));
    signals.push_back(std::move(mockSignal2));
    
    EXPECT_CALL(*mockFactory, createSignals())
        .WillOnce(Return(ByMove(std::move(signals))));
    
    EXPECT_CALL(*mockSignalPtr1, start()).Times(1);
    EXPECT_CALL(*mockSignalPtr2, start()).Times(1);
    
    manager->startSignals();
}

TEST_F(KeyboardSignalManagerTest, StopSignals_CallsStopOnAllSignals) {
    auto mockSignal1 = std::make_unique<MockSignal>();
    auto mockSignal2 = std::make_unique<MockSignal>();
    auto* mockSignalPtr1 = mockSignal1.get();
    auto* mockSignalPtr2 = mockSignal2.get();
    
    std::vector<std::unique_ptr<ISignal>> signals;
    signals.push_back(std::move(mockSignal1));
    signals.push_back(std::move(mockSignal2));
    
    EXPECT_CALL(*mockFactory, createSignals())
        .WillOnce(Return(ByMove(std::move(signals))));
    
    EXPECT_CALL(*mockSignalPtr1, start()).Times(1);
    EXPECT_CALL(*mockSignalPtr2, start()).Times(1);
    EXPECT_CALL(*mockSignalPtr1, stop()).Times(1);
    EXPECT_CALL(*mockSignalPtr2, stop()).Times(1);
    
    manager->startSignals();
    manager->stopSignals();
}

TEST_F(KeyboardSignalManagerTest, CheckSignals_CallsCheckOnAllSignals) {
    auto mockSignal1 = std::make_unique<MockSignal>();
    auto mockSignal2 = std::make_unique<MockSignal>();
    auto* mockSignalPtr1 = mockSignal1.get();
    auto* mockSignalPtr2 = mockSignal2.get();
    
    std::vector<std::unique_ptr<ISignal>> signals;
    signals.push_back(std::move(mockSignal1));
    signals.push_back(std::move(mockSignal2));
    
    EXPECT_CALL(*mockFactory, createSignals())
        .WillOnce(Return(ByMove(std::move(signals))));
    
    EXPECT_CALL(*mockSignalPtr1, start()).Times(1);
    EXPECT_CALL(*mockSignalPtr2, start()).Times(1);
    EXPECT_CALL(*mockSignalPtr1, check(_)).Times(1);
    EXPECT_CALL(*mockSignalPtr2, check(_)).Times(1);
    
    manager->startSignals();
    manager->checkSignals(emptyEvent);
}

TEST_F(KeyboardSignalManagerTest, MultipleOperations_SignalsHandledCorrectly) {
    auto mockSignal = std::make_unique<MockSignal>();
    auto* mockSignalPtr = mockSignal.get();
    
    std::vector<std::unique_ptr<ISignal>> signals;
    signals.push_back(std::move(mockSignal));
    
    EXPECT_CALL(*mockFactory, createSignals())
        .WillOnce(Return(ByMove(std::move(signals))));
    
    {
        InSequence seq;
        EXPECT_CALL(*mockSignalPtr, start()).Times(1);
        EXPECT_CALL(*mockSignalPtr, check(_)).Times(1);
        EXPECT_CALL(*mockSignalPtr, stop()).Times(1);
    }
    
    manager->startSignals();
    manager->checkSignals(emptyEvent);
    manager->stopSignals();
}

TEST_F(KeyboardSignalManagerTest, CheckSignals_WithEvent_PassesEventCorrectly) {
    auto mockSignal = std::make_unique<MockSignal>();
    auto* mockSignalPtr = mockSignal.get();
    
    std::vector<std::unique_ptr<ISignal>> signals;
    signals.push_back(std::move(mockSignal));
    
    EXPECT_CALL(*mockFactory, createSignals())
        .WillOnce(Return(ByMove(std::move(signals))));
    
    std::any testEvent = 42;
    EXPECT_CALL(*mockSignalPtr, start()).Times(1);
    EXPECT_CALL(*mockSignalPtr, check(Truly([](const std::any& event) {
        return std::any_cast<int>(event) == 42;
    }))).Times(1);
    
    manager->startSignals();
    manager->checkSignals(testEvent);
}

TEST_F(KeyboardSignalManagerTest, CheckSignalsWithComplexEvent_EventIsProperlyPassed) {
    struct ComplexEvent {
        int id;
        std::string name;
        bool operator==(const ComplexEvent& other) const {
            return id == other.id && name == other.name;
        }
    };
    
    ComplexEvent event{42, "test"};
    std::any complexEvent = event;
    
    auto mockSignal = std::make_unique<MockSignal>();
    auto* mockSignalPtr = mockSignal.get();
    
    std::vector<std::unique_ptr<ISignal>> signals;
    signals.push_back(std::move(mockSignal));
    
    EXPECT_CALL(*mockFactory, createSignals())
        .WillOnce(Return(ByMove(std::move(signals))));
    
    EXPECT_CALL(*mockSignalPtr, start()).Times(1);
    EXPECT_CALL(*mockSignalPtr, check(Truly([&event](const std::any& receivedEvent) {
        try {
            const auto& received = std::any_cast<ComplexEvent>(receivedEvent);
            return received.id == event.id && received.name == event.name;
        } catch (const std::bad_any_cast&) {
            return false;
        }
    }))).Times(1);
    
    manager->startSignals();
    manager->checkSignals(complexEvent);
}

TEST_F(KeyboardSignalManagerTest, EmptySignalManager_NoErrors) {
    // Mock factory to return empty signals vector
    EXPECT_CALL(*mockFactory, createSignals())
        .WillOnce(Return(ByMove(std::vector<std::unique_ptr<ISignal>>())));
    
    // This test verifies that operations on an empty signal manager don't cause errors
    EXPECT_NO_THROW(manager->startSignals());
    EXPECT_NO_THROW(manager->stopSignals());
    EXPECT_NO_THROW(manager->checkSignals(emptyEvent));
}

TEST_F(KeyboardSignalManagerTest, FactoryConstructorWithKeyboardApi_WorksCorrectly) {
    // Create a manager with a custom factory and keyboard API
    auto customFactory = std::make_shared<MockSignalFactory>();
    auto customManager = std::make_shared<KeyboardSignalManager>(
        customFactory, std::make_unique<KeyboardApi>());
    
    auto mockSignal = std::make_unique<MockSignal>();
    auto* mockSignalPtr = mockSignal.get();
    
    std::vector<std::unique_ptr<ISignal>> signals;
    signals.push_back(std::move(mockSignal));
    
    EXPECT_CALL(*customFactory, createSignals())
        .WillOnce(Return(ByMove(std::move(signals))));
    
    EXPECT_CALL(*mockSignalPtr, start()).Times(1);
    
    customManager->startSignals();
} 