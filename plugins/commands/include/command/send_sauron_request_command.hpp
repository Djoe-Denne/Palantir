#pragma once

#include "command/icommand.hpp"
#include "plugin_export.hpp"
#include <string>
#include <memory>
#include "application.hpp"
#include <vector>

namespace palantir::command {

class COMMANDS_PLUGIN_API SendSauronRequestCommand : public ICommand {
public:
    explicit SendSauronRequestCommand(const std::string& prompt);
    ~SendSauronRequestCommand() override = default;

    SendSauronRequestCommand(const SendSauronRequestCommand&) = delete;
    auto operator=(const SendSauronRequestCommand&) -> SendSauronRequestCommand& = delete;
    SendSauronRequestCommand(SendSauronRequestCommand&&) = delete;
    auto operator=(SendSauronRequestCommand&&) -> SendSauronRequestCommand& = delete;

    auto execute() const -> void override;
    auto useDebounce() const -> bool override;

private:
    [[nodiscard]] auto loadImagesFromFolder() const -> std::vector<std::string>;

#pragma warning(push)
#pragma warning(disable: 4251)
    std::string prompt_;
    std::shared_ptr<Application> app_{Application::getInstance()};
#pragma warning(pop)
};

} // namespace palantir::command
