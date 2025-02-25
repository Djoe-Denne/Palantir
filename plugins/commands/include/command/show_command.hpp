#pragma once

#include "command/icommand.hpp"
#include "Application.hpp"

namespace interview_cheater::command {

class ShowCommand : public ICommand {
public:
    ShowCommand();
    auto execute() -> void override;
    auto useDebounce() -> bool override;

private:
    Application& app_;
};

} // namespace interview_cheater::command 