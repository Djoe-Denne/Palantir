#pragma once

#include "command/icommand.hpp"
#include "plugin_export.hpp"
#include <string>

namespace palantir::command {

class COMMANDS_PLUGIN_API ClearScreenshotCommand : public ICommand {
public:
    ClearScreenshotCommand() = default;
    ~ClearScreenshotCommand() override = default;

    ClearScreenshotCommand(const ClearScreenshotCommand&) = delete;
    auto operator=(const ClearScreenshotCommand&) -> ClearScreenshotCommand& = delete;
    ClearScreenshotCommand(ClearScreenshotCommand&&) = delete;
    auto operator=(ClearScreenshotCommand&&) -> ClearScreenshotCommand& = delete;

    auto execute() const -> void override;
    auto useDebounce() const -> bool override { return false; }

private:
    [[nodiscard]] auto getScreenshotFolder() const -> std::string;
    auto clearFolder() const -> void;
};

} // namespace palantir::command
