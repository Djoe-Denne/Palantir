#ifndef IWINDOW_HPP
#define IWINDOW_HPP

#include <windows.h>

class IWindow {
public:
    virtual ~IWindow() = default;
    virtual void create(HINSTANCE hInstance) = 0;
    virtual void show() = 0;
    virtual void update() = 0;
    virtual void close() = 0;
    virtual HWND getHandle() const = 0;
    virtual bool isRunning() const = 0;
    virtual void setRunning(bool runningState) = 0;
};

#endif // IWINDOW_HPP