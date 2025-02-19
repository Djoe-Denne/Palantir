#ifndef SHOW_COMMAND_HPP
#define SHOW_COMMAND_HPP

#include "command/icommand.hpp"
#include "window/iwindow.hpp"

namespace interview_cheater::command {
    class ShowCommand : public ICommand {
    public:
        explicit ShowCommand(window::IWindow& window);
        void execute() override;

    private:
        window::IWindow& window_;
    };
}
#endif // SHOW_COMMAND_HPP