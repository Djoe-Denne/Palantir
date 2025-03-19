#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <any>

#include "signal/signal_manager.hpp"
#include "signal/isignal.hpp"
#include "mock/signal/mock_signal.hpp"

using namespace palantir::signal;
using namespace palantir::test;
using namespace testing;

class SignalManagerPublicConstructor : public SignalManager {
public:
    using SignalManager::SignalManager;
};

class SignalManagerTest : public Test {
protected:
    void SetUp() override {
        // Reset the singleton instance before each test
        SignalManager::setInstance(nullptr);
        manager = SignalManager::getInstance();
    }

    void TearDown() override {
        SignalManager::setInstance(nullptr);
    }

    std::shared_ptr<SignalManager> manager;
    std::any emptyEvent;
};

TEST_F(SignalManagerTest, GetInstance_ReturnsSameInstance) {
    auto instance1 = SignalManager::getInstance();
    auto instance2 = SignalManager::getInstance();
    EXPECT_EQ(instance1, instance2);
}

TEST_F(SignalManagerTest, AddSignal_SignalIsAdded) {
    auto mockSignal = std::make_unique<MockSignal>();
    auto* mockSignalPtr = mockSignal.get();
    
    EXPECT_CALL(*mockSignalPtr, check(_)).Times(1);
    
    manager->addSignal(std::move(mockSignal));
    manager->checkSignals(emptyEvent);
}

TEST_F(SignalManagerTest, StartSignals_CallsStartOnAllSignals) {
    auto mockSignal1 = std::make_unique<MockSignal>();
    auto mockSignal2 = std::make_unique<MockSignal>();
    auto* mockSignalPtr1 = mockSignal1.get();
    auto* mockSignalPtr2 = mockSignal2.get();
    
    EXPECT_CALL(*mockSignalPtr1, start()).Times(1);
    EXPECT_CALL(*mockSignalPtr2, start()).Times(1);
    
    manager->addSignal(std::move(mockSignal1));
    manager->addSignal(std::move(mockSignal2));
    manager->startSignals();
}

TEST_F(SignalManagerTest, StopSignals_CallsStopOnAllSignals) {
    auto mockSignal1 = std::make_unique<MockSignal>();
    auto mockSignal2 = std::make_unique<MockSignal>();
    auto* mockSignalPtr1 = mockSignal1.get();
    auto* mockSignalPtr2 = mockSignal2.get();
    
    EXPECT_CALL(*mockSignalPtr1, stop()).Times(1);
    EXPECT_CALL(*mockSignalPtr2, stop()).Times(1);
    
    manager->addSignal(std::move(mockSignal1));
    manager->addSignal(std::move(mockSignal2));
    manager->stopSignals();
}

TEST_F(SignalManagerTest, CheckSignals_CallsCheckOnAllSignals) {
    auto mockSignal1 = std::make_unique<MockSignal>();
    auto mockSignal2 = std::make_unique<MockSignal>();
    auto* mockSignalPtr1 = mockSignal1.get();
    auto* mockSignalPtr2 = mockSignal2.get();
    
    EXPECT_CALL(*mockSignalPtr1, check(_)).Times(1);
    EXPECT_CALL(*mockSignalPtr2, check(_)).Times(1);
    
    manager->addSignal(std::move(mockSignal1));
    manager->addSignal(std::move(mockSignal2));
    manager->checkSignals(emptyEvent);
}

TEST_F(SignalManagerTest, MultipleOperations_SignalsHandledCorrectly) {
    auto mockSignal = std::make_unique<MockSignal>();
    auto* mockSignalPtr = mockSignal.get();
    
    {
        InSequence seq;
        EXPECT_CALL(*mockSignalPtr, start()).Times(1);
        EXPECT_CALL(*mockSignalPtr, check(_)).Times(1);
        EXPECT_CALL(*mockSignalPtr, stop()).Times(1);
    }
    
    manager->addSignal(std::move(mockSignal));
    manager->startSignals();
    manager->checkSignals(emptyEvent);
    manager->stopSignals();
}

TEST_F(SignalManagerTest, CheckSignals_WithEvent_PassesEventCorrectly) {
    auto mockSignal = std::make_unique<MockSignal>();
    auto* mockSignalPtr = mockSignal.get();
    
    std::any testEvent = 42;
    EXPECT_CALL(*mockSignalPtr, check(Truly([](const std::any& event) {
        return std::any_cast<int>(event) == 42;
    }))).Times(1);
    
    manager->addSignal(std::move(mockSignal));
    manager->checkSignals(testEvent);
}

TEST_F(SignalManagerTest, SetInstance_CustomInstanceIsUsed) {
    auto customManager = std::make_shared<SignalManagerPublicConstructor>();
    SignalManager::setInstance(customManager);
    
    auto retrievedInstance = SignalManager::getInstance();
    EXPECT_EQ(customManager, retrievedInstance);
}

TEST_F(SignalManagerTest, AddMultipleSignals_AllSignalsAreAdded) {
    auto mockSignal1 = std::make_unique<MockSignal>();
    auto mockSignal2 = std::make_unique<MockSignal>();
    auto mockSignal3 = std::make_unique<MockSignal>();
    
    auto* mockSignalPtr1 = mockSignal1.get();
    auto* mockSignalPtr2 = mockSignal2.get();
    auto* mockSignalPtr3 = mockSignal3.get();
    
    EXPECT_CALL(*mockSignalPtr1, check(_)).Times(1);
    EXPECT_CALL(*mockSignalPtr2, check(_)).Times(1);
    EXPECT_CALL(*mockSignalPtr3, check(_)).Times(1);
    
    manager->addSignal(std::move(mockSignal1));
    manager->addSignal(std::move(mockSignal2));
    manager->addSignal(std::move(mockSignal3));
    
    manager->checkSignals(emptyEvent);
}

TEST_F(SignalManagerTest, CheckSignalsWithComplexEvent_EventIsProperlyPassed) {
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
    
    EXPECT_CALL(*mockSignalPtr, check(Truly([&event](const std::any& receivedEvent) {
        try {
            const auto& received = std::any_cast<ComplexEvent>(receivedEvent);
            return received.id == event.id && received.name == event.name;
        } catch (const std::bad_any_cast&) {
            return false;
        }
    }))).Times(1);
    
    manager->addSignal(std::move(mockSignal));
    manager->checkSignals(complexEvent);
}

TEST_F(SignalManagerTest, EmptySignalManager_NoErrors) {
    // This test verifies that operations on an empty signal manager don't cause errors
    EXPECT_NO_THROW(manager->startSignals());
    EXPECT_NO_THROW(manager->stopSignals());
    EXPECT_NO_THROW(manager->checkSignals(emptyEvent));
} 