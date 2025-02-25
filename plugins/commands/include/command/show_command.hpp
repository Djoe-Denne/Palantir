#pragma once

#include <memory>
#include "command/icommand.hpp"
#include "Application.hpp"

#ifdef _WIN32
    #ifdef COMMANDS_PLUGIN_EXPORTS
        #define COMMANDS_PLUGIN_API __declspec(dllexport)
    #else
        #define COMMANDS_PLUGIN_API __declspec(dllimport)
    #endif
#else
    #define COMMANDS_PLUGIN_API
#endif

namespace interview_cheater::command {

class COMMANDS_PLUGIN_API ShowCommand : public ICommand {
public:
    ShowCommand();
    auto execute() -> void override;
    auto useDebounce() -> bool override;

private:
    std::shared_ptr<Application> app_;
};

} // namespace interview_cheater::command 