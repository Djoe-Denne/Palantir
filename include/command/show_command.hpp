#ifndef SHOW_COMMAND_HPP
#define SHOW_COMMAND_HPP

#include "command/icommand.hpp"
#include "window/iwindow.hpp"

class ShowCommand : public ICommand {
public:
    ShowCommand(IWindow& window);
    void execute() override;

private:
    IWindow& window_;
};

#endif // SHOW_COMMAND_HPP