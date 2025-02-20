#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP

#include <memory>
#include <vector>

namespace interview_cheater
{
namespace command
{
// Forward declaration for ICommand
class ICommand;
}  // namespace command

namespace window
{
// Forward declaration for IWindow
class IWindow;

class WindowManager
{
public:
    WindowManager()  = default;
    ~WindowManager() = default;

    void     addWindow(std::unique_ptr<IWindow> window);
    void     removeWindow(const IWindow* window);
    IWindow* getFirstWindow() const;
    bool     hasRunningWindows() const;
    void     executeCommand(std::unique_ptr<interview_cheater::command::ICommand> command);

private:
    std::vector<std::unique_ptr<IWindow>> windows;
};
}  // namespace window
}  // namespace interview_cheater

#endif  // WINDOW_MANAGER_HPP