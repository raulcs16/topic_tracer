
#include "command_factory.hpp"
#include "file_commands.hpp"
#include "graph_commands.hpp"
#include "ui_commands.hpp"


CommandFactory::CommandFactory(CommandContext *ctx) : m_ctx(ctx) {
    auto add = [this](std::unique_ptr<ICommand> cmd) {
        m_commands.emplace(cmd->name(), std::move(cmd));
    };
    QStringList list;
    add(std::make_unique<LoadCommand>(m_ctx, list));
    add(std::make_unique<SaveCommand>(m_ctx, list));
    add(std::make_unique<LsCommand>(m_ctx, list));
    add(std::make_unique<ClearCommand>(m_ctx, list));
    add(std::make_unique<TouchCommand>(m_ctx, list));
    add(std::make_unique<RmCommand>(m_ctx, list));
    add(std::make_unique<MvCommand>(m_ctx, list));
    add(std::make_unique<LinkCommand>(m_ctx, list));
    add(std::make_unique<UnLinkCommand>(m_ctx, list));
    add(std::make_unique<PathCommand>(m_ctx, list));
    add(std::make_unique<ClearPathCommand>(m_ctx, list));
    add(std::make_unique<FocusCommand>(m_ctx, list));
    add(std::make_unique<ModeCommand>(m_ctx, list));
}

// The "NO" sub-parsing fix
// m_registry["no"] = [this](auto &p) {
//     if (p.size() < 2)
//         return std::unique_ptr<ICommand>(nullptr);
//     if (p[1] == "link")
//         return std::make_unique<UnLinkCommand>(m_ctx->graph, p);
//     if (p[1] == "path")
//         return std::make_unique<ClearPathCommand>(m_ctx->uiContext->store());
//     return std::unique_ptr<ICommand>(nullptr);
// };

std::unique_ptr<ICommand> CommandFactory::create(const QStringList &parts) {
    if (parts.empty())
        return nullptr;

    QString cmd = parts.first().toLower();
    auto it = m_commands.find(cmd);
    if (it != m_commands.end()) {
        return it->second->clone(parts);
    }
    return nullptr;
}

QStringList CommandFactory::availableCommands() {
    QStringList names;
    for (const auto &[name, _] : m_commands) {
        names << name;
    }
    return names;
}

QList<AutoSuggestion> CommandFactory::findMatch(const QString &input) {
    QStringList parts = input.split(" ", Qt::SkipEmptyParts);
    QList<AutoSuggestion> results;

    if (parts.isEmpty())
        return results;

    // Detect if the user just typed a space at the end (e.g., "link ")
    bool endsWithSpace = input.endsWith(" ");
    QString partial = endsWithSpace ? "" : parts.last();
    int startIndex = endsWithSpace ? input.length() : input.lastIndexOf(partial);

    // PHASE 1: Command Name Completion
    if (parts.size() == 1 && !endsWithSpace) {
        for (const auto &[name, _] : m_commands) {
            if (name.startsWith(partial, Qt::CaseInsensitive)) {
                results.append({startIndex, name});
            }
        }
    } else {
        QString cmdName = parts.first().toLower();
        if (m_commands.contains(cmdName)) {
            QStringList candidates = m_commands[cmdName]->getValidArgs(parts);
            for (const auto &candidate : candidates) {
                if (candidate.startsWith(partial, Qt::CaseInsensitive)) {
                    results.append({startIndex, candidate});
                }
            }
        }
    }
    return results;
}
