#ifndef STOP_COMMAND_HPP
#define STOP_COMMAND_HPP

#include "command/icommand.hpp"

namespace interview_cheater {
class Application;  // Forward declaration

namespace command {
class StopCommand final : public ICommand {
public:
    explicit StopCommand(Application& app);
    ~StopCommand() override = default;

    // Delete copy operations
    StopCommand(const StopCommand&) = delete;
    auto operator=(const StopCommand&) -> StopCommand& = delete;

    // Define move operations
    StopCommand(StopCommand&&) noexcept = default;
    auto operator=(StopCommand&&) noexcept -> StopCommand& = delete;

    auto execute() -> void override;
    [[nodiscard]] auto getName() const -> const std::string& override;

private:
    Application& app_;
    const std::string name_{"stop"};
};
}  // namespace command
}  // namespace interview_cheater
#endif  // STOP_COMMAND_HPP