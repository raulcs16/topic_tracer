#pragma once
#include "command_context.hpp"
#include "icommand.hpp"
#include <map>


class CommandFactory {
public:
    CommandFactory(CommandContext *ctx);
    std::unique_ptr<ICommand> create(const QStringList &parts);

    QStringList availableCommands();


private:
private:
    CommandContext *m_ctx;
    std::vector<std::unique_ptr<ICommand>> m_commands;
};