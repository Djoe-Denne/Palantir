#ifndef SIGNAL_MANAGER_HPP
#define SIGNAL_MANAGER_HPP

#include <vector>
#include <memory>
#include "signal/isignal.hpp"

class SignalManager {
public:
    void addSignal(std::unique_ptr<ISignal> signal);
    void startSignals();

private:
    std::vector<std::unique_ptr<ISignal>> signals_;
};

#endif // SIGNAL_MANAGER_HPP