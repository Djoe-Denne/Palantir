#pragma once

#include "command/command_factory.hpp"

#include <unordered_map>

#include "command/icommand.hpp"

namespace palantir::command {

class CommandFactory::CommandFactoryImpl {
public:
    CommandFactoryImpl() = default;
    ~CommandFactoryImpl() = default;
    CommandFactoryImpl(const CommandFactoryImpl&) = delete;
    auto operator=(const CommandFactoryImpl&) -> CommandFactoryImpl& = delete;
    CommandFactoryImpl(CommandFactoryImpl&&) = delete;
    auto operator=(CommandFactoryImpl&&) -> CommandFactoryImpl& = delete;

    std::unordered_map<std::string, CommandFactory::CommandCreator> commands_;

    auto registerCommand(const std::string& commandName, CommandFactory::CommandCreator creator) -> void {
        commands_[commandName] = creator;
    }

    [[nodiscard]] auto unregisterCommand(const std::string& commandName) -> bool {
        return commands_.erase(commandName) > 0;
    }

    auto getCommand(const std::string& name) -> std::unique_ptr<ICommand> {
        auto maybeCommand = commands_.find(name);
        return maybeCommand != commands_.end() ? std::move(maybeCommand->second()) : nullptr;
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

auto CommandFactory::unregisterCommand(const std::string& commandName) -> bool {
    return pimpl_->unregisterCommand(commandName);
}

auto CommandFactory::getCommand(const std::string& name) -> std::unique_ptr<ICommand> {
    return std::move(pimpl_->getCommand(name));
}

}  // namespace palantir::command
