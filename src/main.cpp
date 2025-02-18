#include <memory>
#include "window/window_manager.hpp"
#include "signal/signal_manager.hpp"
#include "signal/toggle_signal.hpp"
#include "window/overlay_window.hpp"
#include "input/input_factory.hpp"
#include "command/show_command.hpp"
#include "platform/application.hpp"

// Platform-agnostic application code
int run_app() {
    WindowManager manager;
    SignalManager signalManager;
    
    auto window = std::make_unique<OverlayWindow>();
    window->create();
    manager.addWindow(std::move(window));

    auto showCmd = std::make_unique<ShowCommand>(*(manager.getFirstWindow()));
    manager.executeCommand(std::move(showCmd));
    
    auto input = InputFactory::create();
    signalManager.addSignal(std::make_unique<ToggleSignal>(manager, *input));
    signalManager.startSignals();
    
    auto app = Application::getInstance(signalManager);
    return app->run();
}

// Entry point that works for both Windows and macOS
#if defined(_WIN32) && !defined(_CONSOLE)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    return run_app();
}
#else
int main(int argc, char* argv[]) {
    return run_app();
}
#endif