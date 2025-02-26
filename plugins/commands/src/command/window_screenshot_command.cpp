#include "command/window_screenshot_command.hpp"
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;
namespace interview_cheater::command {

WindowScreenshotCommand::WindowScreenshotCommand() {
    fs::create_directories("./screenshot");
}

auto WindowScreenshotCommand::generateFilePath() const -> std::string {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::ostringstream oss;
    oss << "./screenshot/screenshot_"
        << std::put_time(std::localtime(&time), "%Y-%m-%d_%H-%M-%S") << ".png";
    return oss.str();
}

// Calls platform-specific implementation
auto WindowScreenshotCommand::execute() -> void {
    captureScreenshot();
}

} // namespace interview_cheater::command
