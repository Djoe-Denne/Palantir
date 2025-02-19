#ifndef SIGNAL_MANAGER_HPP
#define SIGNAL_MANAGER_HPP

#include <memory>
#include <vector>

namespace interview_cheater::signal
{
// Forward declaration for ISignal
class ISignal;

class SignalManager
{
public:
    void addSignal(std::unique_ptr<ISignal> signal);
    void startSignals();
    void stopSignals();
    void checkSignals();

private:
    std::vector<std::unique_ptr<ISignal>> signals_;
};

}  // namespace interview_cheater::signal

#endif  // SIGNAL_MANAGER_HPP