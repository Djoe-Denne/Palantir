#pragma once

#include <memory>
#include "command/icommand.hpp"
#include "Application.hpp"
#include "plugin_export.hpp"

namespace interview_cheater::command {

class COMMANDS_PLUGIN_API ToggleTransparencyCommand : public ICommand {
public:
    ToggleTransparencyCommand();
    ~ToggleTransparencyCommand() override = default;
    
    // Rule of 5
    ToggleTransparencyCommand(const ToggleTransparencyCommand&) = delete;
    auto operator=(const ToggleTransparencyCommand&) -> ToggleTransparencyCommand& = delete;
    ToggleTransparencyCommand(ToggleTransparencyCommand&&) = delete;
    auto operator=(ToggleTransparencyCommand&&) -> ToggleTransparencyCommand& = delete;
    auto execute() -> void override;
    auto useDebounce() -> bool override;

private:
#pragma warning(push)
#pragma warning(disable: 4251)
    std::shared_ptr<Application> app_;
#pragma warning(pop)

    auto getTransparency() -> int;
};

} // namespace interview_cheater::command 