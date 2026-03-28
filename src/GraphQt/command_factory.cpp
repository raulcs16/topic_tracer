#include "clear_command.hpp"
#include "command_factory.hpp"
#include "load_command.hpp"
#include "mv_command.hpp"
#include "rm_command.hpp"
#include "save_command.hpp"
#include "touch_command.hpp"


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
    }


    return nullptr;
}