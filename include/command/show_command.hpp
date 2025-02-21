#ifndef SHOW_COMMAND_HPP
#define SHOW_COMMAND_HPP

#include "command/icommand.hpp"

namespace interview_cheater {
namespace window {
class IWindow;  // Forward declaration
class WindowManager;
}  // namespace window

namespace command {
class ShowCommand final : public ICommand {
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
    [[nodiscard]] auto getName() const -> const std::string& override;

private:
    window::WindowManager& manager_;
    const std::string name_{"toggle"};
};
}  // namespace command
}  // namespace interview_cheater
#endif  // SHOW_COMMAND_HPP