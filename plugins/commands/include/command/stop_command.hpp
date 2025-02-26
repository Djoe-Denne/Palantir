#pragma once

#include <memory>
#include "command/icommand.hpp"
#include "Application.hpp"
#include "plugin_export.hpp"

namespace interview_cheater::command {

class COMMANDS_PLUGIN_API StopCommand : public ICommand {
public:
    StopCommand();
    ~StopCommand() override = default;
    
    // Rule of 5
    StopCommand(const StopCommand&) = delete;
    StopCommand& operator=(const StopCommand&) = delete;
    StopCommand(StopCommand&&) = delete;
    StopCommand& operator=(StopCommand&&) = delete;
    auto execute() -> void override;
    auto useDebounce() -> bool override;

private:
#pragma warning(push)
#pragma warning(disable: 4251)
    std::shared_ptr<Application> app_;
#pragma warning(pop)
};

} // namespace interview_cheater::command 