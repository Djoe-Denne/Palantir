#pragma once

#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
};

#endif // ICOMMAND_HPP