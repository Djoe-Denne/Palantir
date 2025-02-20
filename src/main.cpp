#include <memory>

#include "command/show_command.hpp"
#include "input/input_factory.hpp"
#include "platform/application.hpp"
#include "signal/signal_manager.hpp"
#include "signal/toggle_signal.hpp"
#include "window/overlay_window.hpp"
#include "window/window_manager.hpp"

#if defined(_WIN32) && !defined(_CONSOLE)
#include <windows.h>
#endif

using interview_cheater::Application;
using interview_cheater::command::ShowCommand;
using interview_cheater::input::InputFactory;
using interview_cheater::signal::SignalManager;
using interview_cheater::signal::ToggleSignal;
using interview_cheater::window::OverlayWindow;
using interview_cheater::window::WindowManager;

// Platform-agnostic application code
auto run_app() -> int {
    WindowManager manager;
    SignalManager signalManager;

    auto window = std::make_unique<OverlayWindow>();
    window->create();
    manager.addWindow(std::move(window));

    auto showCmd = std::make_unique<ShowCommand>(manager);
    manager.executeCommand(std::move(showCmd));

    auto input = InputFactory::createInput();
    signalManager.addSignal(std::make_unique<ToggleSignal>(manager, *input));
    signalManager.startSignals();

    auto app = Application::getInstance(signalManager);
    return app->run();
}

// Entry point that works for both Windows and macOS
#if defined(_WIN32) && !defined(_CONSOLE)
// Entry point for Windows applications
auto WINAPI
WinMain(HINSTANCE hInstance /* handle to current instance */  // NOLINT(bugprone-easily-swappable-parameters)
        ,                                                     // NOLINT(bugprone-easily-swappable-parameters)
        HINSTANCE hPrevInstance /* handle to previous instance */, LPSTR lpCmdLine /* command line */,
        int nCmdShow /* show state */) -> int {
    return run_app();
}
#else
int main(int argc, char* argv[]) { return run_app(); }
#endif