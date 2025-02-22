#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <functional>
#include <string>
#include <utility>

#include "icommand.hpp"

namespace interview_cheater::command {

class Command final : public ICommand {
public:
    explicit Command(std::string name, std::function<void()> action)
        : name_(std::move(name)), action_(std::move(action)) {}

    ~Command() override = default;

    // Delete copy operations
    Command(const Command&) = delete;
    auto operator=(const Command&) -> Command& = delete;

    // Define move operations
    Command(Command&&) noexcept = default;
    auto operator=(Command&&) noexcept -> Command& = default;

    auto execute() -> void override { action_(); }

    [[nodiscard]] auto getName() const -> const std::string& override { return name_; }

private:
    std::string name_;
    std::function<void()> action_;
};

}  // namespace interview_cheater::command

#endif  // COMMAND_HPP