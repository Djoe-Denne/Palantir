#include <memory>
#include <stdexcept>
#include <string>
#include <filesystem>

#include "application.hpp"
#include "utils/logger.hpp"
#include "window/overlay_window.hpp"
#include "plugin_loader/plugin_manager.hpp"

#include "platform_application.hpp"

#if defined(_WIN32) && !defined(_CONSOLE)
#include <windows.h>
#endif

using interview_cheater::Application;
using interview_cheater::PlatformApplication;
using interview_cheater::window::OverlayWindow;
using interview_cheater::plugin::PluginManager;

// Platform-agnostic application code
auto run_app() -> int {
    try {
        // Create and initialize application
        auto app = Application::getInstance<PlatformApplication>("config/shortcuts.ini");

        // Initialize plugin manager and load plugins
        auto pluginManager = std::make_unique<PluginManager>();
        
        // Get the executable path and construct plugins directory path
        std::filesystem::path exePath = std::filesystem::current_path();
        std::filesystem::path pluginsDir = exePath / "plugins";

        // Load all plugins from the plugins directory
        pluginManager->setupFromDirectory(pluginsDir);

        // Create window
        auto windowManager = app->getWindowManager();
        auto window = std::make_unique<OverlayWindow>();
        window->create();
        windowManager->addWindow(std::move(window));

        // Attach signals from configuration
        app->attachSignals();

        int result = app->run();

        // Cleanup plugins before exit
        pluginManager->shutdownAll();
        return result;

    } catch (const std::exception& e) {
        DEBUG_LOG("Fatal error: {}", e.what());
#ifdef _WIN32
        MessageBoxA(nullptr, e.what(), "Fatal Error", MB_OK | MB_ICONERROR);
#endif
        return 1;
    }
}

// Entry point that works for both Windows and macOS
#if defined(_WIN32) && !defined(_CONSOLE)
// Entry point for Windows applications
auto WINAPI WinMain(HINSTANCE hInstance /* handle to current instance */,  // NOLINT
                    HINSTANCE hPrevInstance /* handle to previous instance */, LPSTR lpCmdLine /* command line */,
                    int nCmdShow /* show state */) -> int {
    return run_app();
}
#else
auto main(int argc, char* argv[]) -> int { return run_app(); }
#endif