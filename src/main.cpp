#include <windows.h>
#include <memory>
#include "window/window_manager.hpp"
#include "signal/signal_manager.hpp"
#include "signal/ctrl_f1_signal.hpp"
#include "window/overlay_window.hpp"
#include "input/input.hpp"
#include "command/show_command.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WindowManager manager;
    SignalManager signalManager;
    
    auto window = std::make_unique<OverlayWindow>();
    window->create(hInstance);
    manager.addWindow(std::move(window));

    auto showCmd = std::make_unique<ShowCommand>(*(manager.getFirstWindow()));
    manager.executeCommand(std::move(showCmd));
    
    auto input = std::make_unique<Input>();
    signalManager.addSignal(std::make_unique<CtrlF1Signal>(manager, *input));
    signalManager.startSignals();
    
    MSG msg;
    while (manager.hasRunningWindows() && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}