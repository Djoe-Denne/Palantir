#pragma once

#include <memory>
#include "command/icommand.hpp"
#include "Application.hpp"
#include "plugin_export.hpp"

namespace palantir::command {

class COMMANDS_PLUGIN_API StopCommand : public ICommand {
public:
    StopCommand();
    ~StopCommand() override = default;
    
    // Rule of 5
    StopCommand(const StopCommand&) = delete;
    auto operator=(const StopCommand&) -> StopCommand& = delete;
    StopCommand(StopCommand&&) = delete;
    auto operator=(StopCommand&&) -> StopCommand& = delete;
    auto execute() -> void override;
    auto useDebounce() -> bool override;

private:
#pragma warning(push)
#pragma warning(disable: 4251)
    std::shared_ptr<Application> app_;
#pragma warning(pop)
};

} // namespace palantir::command 