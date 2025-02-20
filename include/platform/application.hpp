#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>

namespace interview_cheater {
namespace signal {
// Forward declaration for SignalManager
class SignalManager;
}  // namespace signal

class Application {
public:
    virtual ~Application() = default;

    // Delete copy operations
    Application(const Application&) = delete;
    auto operator=(const Application&) -> Application& = delete;

    // Define move operations
    Application(Application&&) noexcept = default;
    auto operator=(Application&&) noexcept -> Application& = default;

    // Get the singleton instance
    static auto getInstance(signal::SignalManager& signalManager) -> std::shared_ptr<Application>;

    // Platform-agnostic application lifecycle methods
    virtual auto run() -> int = 0;
    virtual auto quit() -> void = 0;

protected:
    Application() = default;

private:
    static std::shared_ptr<Application> instance_;
};
}  // namespace interview_cheater
#endif  // APPLICATION_HPP