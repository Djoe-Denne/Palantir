#pragma once

#include <memory>
#include "command/icommand.hpp"
#include "Application.hpp"
#include "plugin_export.hpp"

namespace interview_cheater::command {

class COMMANDS_PLUGIN_API ShowCommand : public ICommand {
public:
    ShowCommand();
    ~ShowCommand() override {
        app_.reset();
    }
    auto execute() -> void override;
    auto useDebounce() -> bool override;

private:
    std::shared_ptr<Application> app_;
};

} // namespace interview_cheater::command 