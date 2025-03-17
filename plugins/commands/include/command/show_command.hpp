#pragma once

#include <memory>
#include "command/icommand.hpp"
#include "application.hpp"
#include "plugin_export.hpp"

namespace palantir::command {

class COMMANDS_PLUGIN_API ShowCommand : public ICommand {
public:
    ShowCommand() = default;
    ~ShowCommand() override = default;
    
    // Rule of 5
    ShowCommand(const ShowCommand&) = delete;
    auto operator=(const ShowCommand&) -> ShowCommand& = delete;
    ShowCommand(ShowCommand&&) = delete;
    auto operator=(ShowCommand&&) -> ShowCommand& = delete;
    auto execute() const -> void override;
    auto useDebounce() const -> bool override;

private:
#pragma warning(push)
#pragma warning(disable: 4251)
    std::shared_ptr<Application> app_{Application::getInstance()};
#pragma warning(pop)
};

} // namespace palantir::command 