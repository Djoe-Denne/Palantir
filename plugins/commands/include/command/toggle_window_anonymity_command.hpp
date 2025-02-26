#pragma once

#include <memory>
#include "command/icommand.hpp"
#include "Application.hpp"
#include "plugin_export.hpp"

namespace interview_cheater::command {

class COMMANDS_PLUGIN_API ToggleWindowAnonymityCommand : public ICommand {
public:
    ToggleWindowAnonymityCommand();
    ~ToggleWindowAnonymityCommand() override = default;
    
    // Rule of 5
    ToggleWindowAnonymityCommand(const ToggleWindowAnonymityCommand&) = delete;
    auto operator=(const ToggleWindowAnonymityCommand&) -> ToggleWindowAnonymityCommand& = delete;
    ToggleWindowAnonymityCommand(ToggleWindowAnonymityCommand&&) = delete;
    auto operator=(ToggleWindowAnonymityCommand&&) -> ToggleWindowAnonymityCommand& = delete;
    auto execute() -> void override;
    auto useDebounce() -> bool override;

private:
#pragma warning(push)
#pragma warning(disable: 4251)
    std::shared_ptr<Application> app_;
#pragma warning(pop)
};

} // namespace interview_cheater::command 