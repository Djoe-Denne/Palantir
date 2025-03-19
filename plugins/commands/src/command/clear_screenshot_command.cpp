#include "command/clear_screenshot_command.hpp"
#include <filesystem>
#include <iostream>
#include <utils/logger.hpp>

namespace palantir::command {

auto ClearScreenshotCommand::getScreenshotFolder() const -> std::string {
    return "./screenshot";
}

auto ClearScreenshotCommand::clearFolder() const -> void {
    std::filesystem::path folderPath = getScreenshotFolder();
    if (!std::filesystem::exists(folderPath)) {
        DebugLog("Screenshot folder does not exist: ", folderPath);
        return;
    }

    size_t deletedFiles = 0;
    try {
        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            if (std::filesystem::is_regular_file(entry)) {
                std::filesystem::remove(entry);
                deletedFiles++;
            }
        }
        DebugLog("Cleared ", deletedFiles, " screenshots from ", folderPath);
    } catch (const std::filesystem::filesystem_error& e) {
        DebugLog("Error clearing screenshot folder: ", e.what());
    }
}

auto ClearScreenshotCommand::execute() const -> void { clearFolder(); }

} // namespace palantir::command
