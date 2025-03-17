#pragma once

#include <memory>
#include "command/icommand.hpp"
#include "application.hpp"
#include "plugin_export.hpp"

namespace palantir::command {

class COMMANDS_PLUGIN_API ToggleTransparencyCommand : public ICommand {
public:
    ToggleTransparencyCommand() = default;
    ~ToggleTransparencyCommand() override = default;
    
    // Rule of 5
    ToggleTransparencyCommand(const ToggleTransparencyCommand&) = delete;
    auto operator=(const ToggleTransparencyCommand&) -> ToggleTransparencyCommand& = delete;
    ToggleTransparencyCommand(ToggleTransparencyCommand&&) = delete;
    auto operator=(ToggleTransparencyCommand&&) -> ToggleTransparencyCommand& = delete;
    auto execute() const -> void override;
    auto useDebounce() const -> bool override;

private:
#pragma warning(push)
#pragma warning(disable: 4251)
    std::shared_ptr<Application> app_{Application::getInstance()};
#pragma warning(pop)

    auto getTransparency() const -> int;
};

} // namespace palantir::command 