#ifndef WINDOWS_OVERLAY_WINDOW_HPP
#define WINDOWS_OVERLAY_WINDOW_HPP

#include "window/iwindow.hpp"

class OverlayWindow : public IWindow {
public:
    OverlayWindow();
    void create(HINSTANCE hInstance) override;
    void show() override;
    void update() override;
    void close() override;
    HWND getHandle() const override;
    bool isRunning() const override;
    void setRunning(bool runningState) override;

private:
    HWND hwnd_;
    bool running_;
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif // WINDOWS_OVERLAY_WINDOW_HPP