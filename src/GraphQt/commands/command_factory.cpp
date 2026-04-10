#include "clear_command.hpp"
#include "clearpath_command.hpp"
#include "command_factory.hpp"
#include "focus_command.hpp"
#include "link_command.hpp"
#include "load_command.hpp"
#include "mode_command.hpp"
#include "mv_command.hpp"
#include "path_command.hpp"
#include "rm_command.hpp"
#include "save_command.hpp"
#include "touch_command.hpp"
#include "unlink_command.hpp"


CommandFactory::CommandFactory(CommandContext *ctx) : m_ctx(ctx) { registerCommands(); }

void CommandFactory::registerCommands() {
    // Basic Commands
    m_registry["clear"] = [this](auto &p) {
        return std::make_unique<ClearCommand>(m_ctx->uiContext->store());
    };
    // File IO
    m_registry["load"] = [this](auto &p) {
        return p.size() >= 2 ? std::make_unique<LoadCommand>(m_ctx, p[1]) : nullptr;
    };
    m_registry["save"] = [this](auto &p) {
        return p.size() >= 2 ? std::make_unique<SaveCommand>(m_ctx, p[1]) : nullptr;
    };

    // Graph Operations
    m_registry["touch"] = [this](auto &p) {
        return std::make_unique<TouchCommand>(m_ctx->graph, p);
    };
    m_registry["rm"] = [this](auto &p) {
        return std::make_unique<RmCommand>(m_ctx->graph, p);
    };
    m_registry["mv"] = [this](auto &p) {
        return std::make_unique<MvCommand>(m_ctx->graph, p);
    };
    m_registry["link"] = [this](auto &p) {
        return std::make_unique<LinkCommand>(m_ctx->graph, p);
    };
    m_registry["path"] = [this](auto &p) {
        return std::make_unique<PathCommand>(m_ctx, p);
    };
    m_registry["focus"] = [this](auto &p) {
        return std::make_unique<FocusCommand>(m_ctx, p);
    };
    m_registry["mode"] = [this](auto &p) {
        return std::make_unique<ModeCommand>(m_ctx->uiContext, p.at(1));
    };
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
}

std::unique_ptr<ICommand> CommandFactory::create(const QStringList &parts) {
    if (parts.empty())
        return nullptr;

    QString cmd = parts.first().toLower();
    if (m_registry.count(cmd)) {
        return m_registry[cmd](parts);
    }
    return nullptr;
}