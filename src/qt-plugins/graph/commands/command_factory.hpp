#pragma once
#include "command_context.hpp"
#include "icommand.hpp"
#include <map>

struct AutoSuggestion {
    int startIndex = -1;
    QString suggestion;
};

class CommandFactory {
public:
    CommandFactory(CommandContext *ctx);
    std::unique_ptr<ICommand> create(const QStringList &parts);
    QStringList availableCommands();
    QList<AutoSuggestion> findMatch(const QString &input);

private:
private:
    CommandContext *m_ctx;
    std::map<QString, std::unique_ptr<ICommand>> m_commands;
};