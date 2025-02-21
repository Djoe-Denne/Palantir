#include "signal_manager.hpp"
#include <algorithm>
#include <mutex>
#include "utils/logger.hpp"

namespace interview_cheater::signal {

/**
 * @brief Implementation details for the signal manager.
 * 
 * This class handles the internal implementation of signal management using the PIMPL idiom.
 * It maintains thread-safe collections of signals and provides mechanisms for signal
 * registration, checking, and cleanup.
 */
class SignalManager::Impl {
   public:
    // Delete copy operations
    Impl(const Impl& other) = delete;
    auto operator=(const Impl& other) -> Impl& = delete;
    // Delete move operations
    Impl(Impl&& other) noexcept = delete;
    auto operator=(Impl&& other) noexcept -> Impl& = delete;

    /**
     * @brief Construct the implementation object.
     * 
     * Initializes an empty signal collection with thread-safe access.
     */
    Impl() { DEBUG_LOG("Initializing signal manager"); }

    /**
     * @brief Register a new signal.
     * @param signal Shared pointer to the signal to register.
     * 
     * Thread-safely adds a new signal to the collection. The signal will be
     * checked during subsequent signal checks.
     */
    auto registerSignal(std::shared_ptr<ISignal> signal) -> void {
        DEBUG_LOG("Registering signal");
        std::lock_guard<std::mutex> lock(mutex_);
        signals_.push_back(std::move(signal));
    }

    /**
     * @brief Check all registered signals.
     * 
     * Thread-safely iterates through all registered signals, checking each one
     * and executing their associated commands if triggered. Signals that are
     * no longer valid are removed from the collection.
     */
    auto checkSignals() -> void {
        std::lock_guard<std::mutex> lock(mutex_);

        // Remove any invalid signals first
        signals_.erase(
            std::remove_if(signals_.begin(), signals_.end(),
                          [](const auto& signal) { return !signal || !signal->isValid(); }),
            signals_.end());

        // Check remaining valid signals
        for (const auto& signal : signals_) {
            if (signal->check()) {
                DEBUG_LOG("Signal triggered");
                signal->executeCommand();
            }
        }
    }

    /**
     * @brief Clean up implementation resources.
     * 
     * Ensures proper cleanup of all registered signals.
     */
    ~Impl() {
        DEBUG_LOG("Destroying signal manager");
        std::lock_guard<std::mutex> lock(mutex_);
        signals_.clear();
    }

   private:
    std::vector<std::shared_ptr<ISignal>> signals_;  ///< Collection of registered signals
    std::mutex mutex_;                               ///< Mutex for thread-safe access
};

/**
 * @brief Construct the signal manager.
 * 
 * Creates a new signal manager instance with its implementation.
 */
SignalManager::SignalManager() : pImpl_(std::make_unique<Impl>()) {
    DEBUG_LOG("Creating signal manager");
}

// Required for unique_ptr with incomplete type
SignalManager::~SignalManager() = default;

/**
 * @brief Register a new signal.
 * @param signal Shared pointer to the signal to register.
 * 
 * Delegates to the implementation's registerSignal method to
 * handle signal registration.
 */
auto SignalManager::registerSignal(std::shared_ptr<ISignal> signal) -> void {
    pImpl_->registerSignal(std::move(signal));
}

/**
 * @brief Check all registered signals.
 * 
 * Delegates to the implementation's checkSignals method to
 * handle signal checking and command execution.
 */
auto SignalManager::checkSignals() -> void { pImpl_->checkSignals(); }

}  // namespace interview_cheater::signal 