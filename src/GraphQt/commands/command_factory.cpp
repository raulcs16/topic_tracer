
#include "command_factory.hpp"
#include "file_commands.hpp"
#include "graph_commands.hpp"
#include "ui_commands.hpp"


CommandFactory::CommandFactory(CommandContext *ctx) : m_ctx(ctx) {
    QStringList list;
    m_commands.push_back(std::make_unique<LoadCommand>(m_ctx, list));
    m_commands.push_back(std::make_unique<SaveCommand>(m_ctx, list));
    m_commands.push_back(std::make_unique<ClearCommand>(m_ctx, list));
    m_commands.push_back(std::make_unique<TouchCommand>(m_ctx, list));
    m_commands.push_back(std::make_unique<RmCommand>(m_ctx, list));
    m_commands.push_back(std::make_unique<MvCommand>(m_ctx, list));
    m_commands.push_back(std::make_unique<LinkCommand>(m_ctx, list));
    m_commands.push_back(std::make_unique<UnLinkCommand>(m_ctx, list));
    m_commands.push_back(std::make_unique<PathCommand>(m_ctx, list));
    m_commands.push_back(std::make_unique<ClearPathCommand>(m_ctx, list));
    m_commands.push_back(std::make_unique<FocusCommand>(m_ctx, list));
    m_commands.push_back(std::make_unique<MvCommand>(m_ctx, list));
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
    if (cmd == "load")
        return std::make_unique<LoadCommand>(m_ctx, parts);
    if (cmd == "save")
        return std::make_unique<SaveCommand>(m_ctx, parts);
    if (cmd == "clear")
        return std::make_unique<ClearCommand>(m_ctx, parts);
    if (cmd == "touch")
        return std::make_unique<TouchCommand>(m_ctx, parts);
    if (cmd == "rm")
        return std::make_unique<RmCommand>(m_ctx, parts);
    if (cmd == "mv")
        return std::make_unique<MvCommand>(m_ctx, parts);
    if (cmd == "link")
        return std::make_unique<LinkCommand>(m_ctx, parts);
    if (cmd == "path")
        return std::make_unique<PathCommand>(m_ctx, parts);
    if (cmd == "focus")
        return std::make_unique<FocusCommand>(m_ctx, parts);
    if (cmd == "mv")
        return std::make_unique<MvCommand>(m_ctx, parts);
    if (cmd == "no path")
        return std::make_unique<ClearPathCommand>(m_ctx, parts);
    if (cmd == "no link")
        return std::make_unique<UnLinkCommand>(m_ctx, parts);
    if (cmd == "help")
        return std::make_unique<HelpCommand>(m_commands);
    return nullptr;
}

QStringList CommandFactory::availableCommands() {
    QStringList names;
    for (const auto &cmd : m_commands) {
        names << cmd->name();
    }
    return names;
}