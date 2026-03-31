#pragma once
#include "command_context.hpp"
#include "icommand.hpp"

class CommandFactory {
public:
    CommandFactory(CommandContext *ctx) : m_ctx(ctx) {}
    std::unique_ptr<ICommand> create(const QStringList &parts);

private:
    CommandContext *m_ctx;
};