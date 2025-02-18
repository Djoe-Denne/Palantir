#include "signal/signal_manager.hpp"

void SignalManager::addSignal(std::unique_ptr<ISignal> signal) {
    signals_.push_back(std::move(signal));
}

void SignalManager::startSignals() {
    for (auto& signal : signals_) {
        signal->start();
    }
}

void SignalManager::stopSignals() {
    for (auto& signal : signals_) {
        signal->stop();
    }
}

void SignalManager::checkSignals() {
    for (auto& signal : signals_) {
        if (signal->isActive()) {
            signal->check();
        }
    }
} 