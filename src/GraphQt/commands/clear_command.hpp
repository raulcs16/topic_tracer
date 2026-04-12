#pragma once

#include "command_context.hpp"
#include "icommand.hpp"


class ClearCommand : public ICommand {
public:
    explicit ClearCommand(CommandContext *context) : m_context(context) {}

    CommandResult execute() override {
        m_context->graph->clear();
        return CommandResult::ok("");
    }
    void undo() override {}

private:
    CommandContext *m_context;
};