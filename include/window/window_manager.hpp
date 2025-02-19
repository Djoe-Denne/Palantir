#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP

#include <memory>
#include <vector>
#include "window/iwindow.hpp"
#include "command/icommand.hpp"

namespace interview_cheater::window {

class WindowManager {
public:
    WindowManager() = default;
    ~WindowManager() = default;
    
    void addWindow(std::unique_ptr<IWindow> window);
    void removeWindow(const IWindow* window);
    IWindow* getFirstWindow() const;
    bool hasRunningWindows() const;
    void executeCommand(std::unique_ptr<command::ICommand> command);

private:
    std::vector<std::unique_ptr<IWindow>> windows;
};

} // namespace interview_cheater::window

#endif // WINDOW_MANAGER_HPP