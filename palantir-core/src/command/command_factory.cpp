#pragma once

#include "command/command_factory.hpp"

#include <unordered_map>

#include "command/icommand.hpp"

namespace interview_cheater::command {

class CommandFactory::CommandFactoryImpl {
public:
    CommandFactoryImpl() = default;
    ~CommandFactoryImpl() = default;

    std::unordered_map<std::string, CommandFactory::CommandCreator> commands_;

    auto registerCommand(const std::string& commandName, CommandFactory::CommandCreator creator) -> void {
        commands_[commandName] = creator;
    }

    auto getCommand(const std::string& name) -> std::unique_ptr<ICommand> {
        auto maybeCommand = commands_.find(name);
        return maybeCommand != commands_.end() ? maybeCommand->second() : nullptr;
    }
};

CommandFactory::CommandFactory() : pimpl_(std::make_unique<CommandFactoryImpl>()) {}

CommandFactory::~CommandFactory() = default;

auto CommandFactory::getInstance() -> CommandFactory& {
    static CommandFactory instance;
    return instance;
}

auto CommandFactory::registerCommand(const std::string& commandName, CommandCreator creator) -> void {
    pimpl_->registerCommand(commandName, creator);
}

auto CommandFactory::getCommand(const std::string& name) -> std::unique_ptr<ICommand> {
    return pimpl_->getCommand(name);
}

}  // namespace interview_cheater::command
