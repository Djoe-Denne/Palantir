#pragma once

#include "command/icommand.hpp"
#include <string>
#include "plugin_export.hpp"

namespace interview_cheater::command {

class COMMANDS_PLUGIN_API WindowScreenshotCommand : public ICommand {
public:
    WindowScreenshotCommand();
    ~WindowScreenshotCommand() override = default;

    WindowScreenshotCommand(const WindowScreenshotCommand&) = delete;
    auto operator=(const WindowScreenshotCommand&) -> WindowScreenshotCommand& = delete;
    WindowScreenshotCommand(WindowScreenshotCommand&&) = delete;
    auto operator=(WindowScreenshotCommand&&) -> WindowScreenshotCommand& = delete;

    void execute() override;
    bool useDebounce() override { return false; }

private:
    std::string generateFilePath() const;
    void captureScreenshot(); // Implemented separately per platform
};

} // namespace interview_cheater::command
