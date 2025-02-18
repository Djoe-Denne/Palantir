#ifndef STOP_COMMAND_HPP
#define STOP_COMMAND_HPP

#include "command/icommand.hpp"
#include "window/iwindow.hpp"

class StopCommand : public ICommand {
public:
    StopCommand(IWindow& window);
    void execute() override;

private:
    IWindow& window_;
};

#endif // STOP_COMMAND_HPP