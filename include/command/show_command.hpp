#ifndef SHOW_COMMAND_HPP
#define SHOW_COMMAND_HPP

#include "command/icommand.hpp"

namespace interview_cheater {
namespace window {
class IWindow;  // Forward declaration
class WindowManager;
}

namespace command {
class ShowCommand : public ICommand {
public:
    explicit ShowCommand(window::WindowManager& manager);
    ~ShowCommand() override = default;

    // Delete copy operations
    ShowCommand(const ShowCommand&) = delete;
    auto operator=(const ShowCommand&) -> ShowCommand& = delete;

    // Define move operations
    ShowCommand(ShowCommand&&) noexcept = default;
    auto operator=(ShowCommand&&) noexcept -> ShowCommand& = delete;

    auto execute() -> void override;

private:
    window::WindowManager& windowManager_;
};
}  // namespace command
}  // namespace interview_cheater
#endif  // SHOW_COMMAND_HPP