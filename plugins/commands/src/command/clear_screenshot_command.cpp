#include "command/clear_screenshot_command.hpp"
#include <filesystem>
#include <iostream>
#include <utils/logger.hpp>

namespace fs = std::filesystem;
namespace palantir::command {

auto ClearScreenshotCommand::getScreenshotFolder() const -> std::string {
    return "./screenshot";
}

auto ClearScreenshotCommand::clearFolder() -> void {
    std::string folderPath = getScreenshotFolder();
    if (!fs::exists(folderPath)) {
        DEBUG_LOG("Screenshot folder does not exist: ", folderPath);
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
        DEBUG_LOG("Cleared ", deletedFiles, " screenshots from ", folderPath);
    } catch (const std::exception& e) {
        DEBUG_LOG("Error clearing screenshot folder: ", e.what());
    }
}

auto ClearScreenshotCommand::execute() -> void { clearFolder(); }

} // namespace palantir::command
