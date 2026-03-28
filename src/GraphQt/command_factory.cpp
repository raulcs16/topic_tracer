#include "clear_command.hpp"
#include "clearpath_command.hpp"
#include "command_factory.hpp"
#include "link_command.hpp"
#include "load_command.hpp"
#include "mv_command.hpp"
#include "path_command.hpp"
#include "rm_command.hpp"
#include "save_command.hpp"
#include "touch_command.hpp"
#include "unlink_command.hpp"


std::unique_ptr<ICommand> CommandFactory::create(const QStringList &parts) {
    if (parts.empty())
        return nullptr;
    QString cmd = parts.first().toLower();
    if (cmd == "clear") {
        return std::make_unique<ClearCommand>(m_ctx->store);
    }
    if (parts.size() >= 2) {
        if (cmd == "load") {
            return std::make_unique<LoadCommand>(m_ctx, parts.at(1));
        }
        if (cmd == "save") {
            return std::make_unique<SaveCommand>(m_ctx, parts.at(1));
        }
        if (cmd == "touch") {
            return std::make_unique<TouchCommand>(m_ctx->graph, parts);
        }
        if (cmd == "rm") {
            return std::make_unique<RmCommand>(m_ctx->graph, parts);
        }
        if (cmd == "mv") {
            return std::make_unique<MvCommand>(m_ctx->graph, parts);
        }
        if (cmd == "link") {
            return std::make_unique<LinkCommand>(m_ctx->graph, parts);
        }
        if (cmd == "path") {
            return std::make_unique<PathCommand>(m_ctx, parts);
        }
        if (cmd == "no") {
            if (parts.at(1) == "link")
                return std::make_unique<UnLinkCommand>(m_ctx->graph, parts);
            if (parts.at(1) == "path") {
                return std::make_unique<ClearPathCommand>(m_ctx->store);
            }
        }
    }


    return nullptr;
}