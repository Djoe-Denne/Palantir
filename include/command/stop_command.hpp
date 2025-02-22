#ifndef STOP_COMMAND_HPP
#define STOP_COMMAND_HPP

#include "command/icommand.hpp"

#include "application.hpp"

namespace interview_cheater::command {
class StopCommand final : public ICommand {
public:
    explicit StopCommand();
    ~StopCommand() override = default;

    // Delete copy operations
    StopCommand(const StopCommand&) = delete;
    auto operator=(const StopCommand&) -> StopCommand& = delete;

    // Delete move operations
    StopCommand(StopCommand&&) noexcept = delete;
    auto operator=(StopCommand&&) noexcept -> StopCommand& = delete;

    auto execute() -> void override;
    auto useDebounce() -> bool override;
private:
    Application& app_;
};
}  // namespace interview_cheater::command

#endif  // STOP_COMMAND_HPP