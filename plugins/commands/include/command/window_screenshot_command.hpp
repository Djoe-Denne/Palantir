#pragma once

#include "command/icommand.hpp"
#include <string>
#include "plugin_export.hpp"

namespace palantir::command {

class COMMANDS_PLUGIN_API WindowScreenshotCommand : public ICommand {
public:
    WindowScreenshotCommand();
    ~WindowScreenshotCommand() override = default;

    WindowScreenshotCommand(const WindowScreenshotCommand&) = delete;
    auto operator=(const WindowScreenshotCommand&) -> WindowScreenshotCommand& = delete;
    WindowScreenshotCommand(WindowScreenshotCommand&&) = delete;
    auto operator=(WindowScreenshotCommand&&) -> WindowScreenshotCommand& = delete;

    auto execute() -> void override;
    auto useDebounce() -> bool override { return false; }

private:
    [[nodiscard]] auto generateFilePath() const -> std::string;
    auto captureScreenshot() -> void; // Implemented separately per platform
};

} // namespace palantir::command
