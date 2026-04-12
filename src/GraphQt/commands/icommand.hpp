#pragma once
#include "command_result.hpp"

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual CommandResult execute() = 0;
    virtual void undo() = 0;

    virtual QString getHint() const = 0;

    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual QString usage() const = 0;
};