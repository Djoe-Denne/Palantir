#pragma once

#include "command/icommand.hpp"
#include "Application.hpp"

namespace interview_cheater::command {

class StopCommand : public ICommand {
public:
    StopCommand();
    auto execute() -> void override;
    auto useDebounce() -> bool override;

private:
    Application& app_;
};

} // namespace interview_cheater::command 