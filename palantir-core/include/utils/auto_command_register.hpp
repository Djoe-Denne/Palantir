#pragma once

#include "command/command_factory.hpp"

#define REGISTER_COMMAND(commandName, name_space, className) \
    namespace { \
        auto create##className() -> std::unique_ptr<interview_cheater::command::ICommand> { \
            return std::make_unique<name_space::className>(); \
        } \
        const bool registered##className = []() { \
            interview_cheater::command::CommandFactory::getInstance().registerCommand(commandName, &create##className); \
            return true; \
        }(); \
    }
