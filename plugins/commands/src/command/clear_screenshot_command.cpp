#include "command/clear_screenshot_command.hpp"
#include <filesystem>
#include <iostream>
#include <utils/logger.hpp>

namespace fs = std::filesystem;
namespace palantir::command {

auto ClearScreenshotCommand::getScreenshotFolder() const -> std::string {
    return "./screenshot";
}

auto ClearScreenshotCommand::clearFolder() const -> void {
    std::string folderPath = getScreenshotFolder();
    if (!fs::exists(folderPath)) {
        DebugLog("Screenshot folder does not exist: ", folderPath);
        return;
    }

    size_t deletedFiles = 0;
    try {
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (fs::is_regular_file(entry)) {
                fs::remove(entry);
                deletedFiles++;
            }
        }
        DebugLog("Cleared ", deletedFiles, " screenshots from ", folderPath);
    } catch (const std::exception& e) {
        DebugLog("Error clearing screenshot folder: ", e.what());
    }
}

auto ClearScreenshotCommand::execute() const -> void { clearFolder(); }

} // namespace palantir::command
