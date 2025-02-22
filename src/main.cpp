#include <memory>
#include <stdexcept>
#include <string>

#include "platform/application.hpp"
#include "utils/logger.hpp"
#include "window/overlay_window.hpp"

#if defined(_WIN32) && !defined(_CONSOLE)
#include <windows.h>
#endif

using interview_cheater::Application;
using interview_cheater::window::OverlayWindow;

// Platform-agnostic application code
auto run_app() -> int {
    try {
        // Create and initialize application
        auto* app = Application::getInstance("config/shortcuts.ini");

        // Create window
        auto& windowManager = app->getWindowManager();
        auto window = std::make_unique<OverlayWindow>();
        window->create();
        windowManager.addWindow(std::move(window));

        // Attach signals from configuration
        app->attachSignals();

        return app->run();
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