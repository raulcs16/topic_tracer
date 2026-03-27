#include "clear_command.hpp"
#include "command_factory.hpp"
#include "load_command.hpp"


std::unique_ptr<ICommand> CommandFactory::create(const QStringList &parts) {
    if (parts.empty())
        return nullptr;
    QString cmd = parts.first().toLower();
    if (cmd == "clear") {
        return std::make_unique<ClearCommand>(m_ctx->store);
    }
    if (cmd == "load" && parts.size() > 1) {
        return std::make_unique<LoadCommand>(m_ctx, parts.at(1));
    }


    return nullptr;
}