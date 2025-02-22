#pragma once

#include "command/command_factory.hpp"

#include "command/icommand.hpp"

namespace interview_cheater::command {

std::unordered_map<std::string, CommandFactory::CommandCreator> CommandFactory::commands_{};

auto CommandFactory::registerCommand(const std::string& commandName, CommandCreator creator) -> void {
    commands_[commandName] = creator;
}

auto CommandFactory::getCommand(const std::string& name) -> std::unique_ptr<ICommand> {
    auto maybeCommand = commands_.find(name);
    return maybeCommand != commands_.end() ? maybeCommand->second() : nullptr;
}

}  // namespace interview_cheater::command
