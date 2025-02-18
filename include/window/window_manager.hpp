#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP

#include <vector>
#include <memory>
#include "window/iwindow.hpp"
#include "command/icommand.hpp"

class WindowManager {
public:
    void addWindow(std::unique_ptr<IWindow> window);
    IWindow* getFirstWindow() const;
    bool hasRunningWindows() const;
    void executeCommand(std::unique_ptr<ICommand> command);

private:
    std::vector<std::unique_ptr<IWindow>> windows_;
};

#endif // WINDOW_MANAGER_HPP