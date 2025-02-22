#ifndef SHOW_COMMAND_HPP
#define SHOW_COMMAND_HPP

#include "command/icommand.hpp"

#include "application.hpp"

namespace interview_cheater::command {
class ShowCommand final : public ICommand {
public:
    explicit ShowCommand();
    ~ShowCommand() override = default;

    // Delete copy operations
    ShowCommand(const ShowCommand&) = delete;
    auto operator=(const ShowCommand&) -> ShowCommand& = delete;

    // Define move operations
    ShowCommand(ShowCommand&&) noexcept = default;
    auto operator=(ShowCommand&&) noexcept -> ShowCommand& = delete;

    auto execute() -> void override;
    
    auto useDebounce() -> bool override;

private:
    Application& app_;
};
}  // namespace interview_cheater::command

#endif  // SHOW_COMMAND_HPP