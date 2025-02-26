#pragma once

#include <memory>
#include "command/icommand.hpp"
#include "Application.hpp"
#include "plugin_export.hpp"

namespace interview_cheater::command {

class COMMANDS_PLUGIN_API ShowCommand : public ICommand {
public:
    ShowCommand();
    ~ShowCommand() override = default;
    
    // Rule of 5
    ShowCommand(const ShowCommand&) = delete;
    ShowCommand& operator=(const ShowCommand&) = delete;
    ShowCommand(ShowCommand&&) = delete;
    ShowCommand& operator=(ShowCommand&&) = delete;
    auto execute() -> void override;
    auto useDebounce() -> bool override;

private:
#pragma warning(push)
#pragma warning(disable: 4251)
    std::shared_ptr<Application> app_;
#pragma warning(pop)
};

} // namespace interview_cheater::command 