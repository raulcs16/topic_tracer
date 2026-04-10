#pragma once
#include "command_result.hpp"

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual CommandResult execute() = 0;
    virtual void undo() = 0;
};