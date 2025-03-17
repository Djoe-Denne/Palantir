#pragma once

#include <memory>
#include "command/icommand.hpp"
#include "application.hpp"
#include "plugin_export.hpp"

namespace palantir::command {

class COMMANDS_PLUGIN_API ToggleWindowAnonymityCommand : public ICommand {
public:
    ToggleWindowAnonymityCommand() = default;
    ~ToggleWindowAnonymityCommand() override = default;
    
    // Rule of 5
    ToggleWindowAnonymityCommand(const ToggleWindowAnonymityCommand&) = delete;
    auto operator=(const ToggleWindowAnonymityCommand&) -> ToggleWindowAnonymityCommand& = delete;
    ToggleWindowAnonymityCommand(ToggleWindowAnonymityCommand&&) = delete;
    auto operator=(ToggleWindowAnonymityCommand&&) -> ToggleWindowAnonymityCommand& = delete;
    auto execute() const -> void override;
    auto useDebounce() const -> bool override;

private:
#pragma warning(push)
#pragma warning(disable: 4251)
    std::shared_ptr<Application> app_{Application::getInstance()};
#pragma warning(pop)
};

} // namespace palantir::command 