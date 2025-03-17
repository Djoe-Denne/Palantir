#pragma once

#include <memory>
#include "command/icommand.hpp"
#include "application.hpp"
#include "plugin_export.hpp"

namespace palantir::command {

class COMMANDS_PLUGIN_API StopCommand : public ICommand {
public:
    StopCommand() = default;
    ~StopCommand() override = default;
    
    // Rule of 5
    StopCommand(const StopCommand&) = delete;
    auto operator=(const StopCommand&) -> StopCommand& = delete;
    StopCommand(StopCommand&&) = delete;
    auto operator=(StopCommand&&) -> StopCommand& = delete;
    auto execute() const -> void override;
    auto useDebounce() const -> bool override;

private:
#pragma warning(push)
#pragma warning(disable: 4251)
    std::shared_ptr<Application> app_{Application::getInstance()};
#pragma warning(pop)
};

} // namespace palantir::command 