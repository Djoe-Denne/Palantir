#include "window/window_manager.hpp"

#include "command/icommand.hpp"
#include "window/iwindow.hpp"

#include <algorithm>

namespace interview_cheater::window
{
void WindowManager::addWindow(std::unique_ptr<IWindow> window)
{
    windows.push_back(std::move(window));
}

void WindowManager::removeWindow(const IWindow* window)
{
    auto iter = std::find_if(windows.begin(), windows.end(), [window](const auto& win) { return win.get() == window; });
    if (iter != windows.end())
    {
        windows.erase(iter);
    }
}

auto WindowManager::getFirstWindow() const -> IWindow*
{
    return windows.empty() ? nullptr : windows.front().get();
}

auto WindowManager::hasRunningWindows() const -> bool
{
    return std::any_of(windows.begin(), windows.end(), [](const auto& window) { return window->isRunning(); });
}

void WindowManager::executeCommand(std::unique_ptr<interview_cheater::command::ICommand> command)
{
    if (command)
    {
        command->execute();
    }
}
}  // namespace interview_cheater::window