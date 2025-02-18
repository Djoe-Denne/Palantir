#include "signal/signal_manager.hpp"

void SignalManager::addSignal(std::unique_ptr<ISignal> signal) {
    signals_.push_back(std::move(signal));
}

void SignalManager::startSignals() {
    for (const auto& signal : signals_) {
        signal->start();
    }
}