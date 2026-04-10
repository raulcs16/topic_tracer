#pragma once
#include "command_context.hpp"
#include "icommand.hpp"
#include <map>

using CommandCreator = std::function<std::unique_ptr<ICommand>(const QStringList &)>;

class CommandFactory {
public:
    CommandFactory(CommandContext *ctx);
    std::unique_ptr<ICommand> create(const QStringList &parts);

private:
    void registerCommands();

private:
    CommandContext *m_ctx;
    std::map<QString, CommandCreator> m_registry;
};