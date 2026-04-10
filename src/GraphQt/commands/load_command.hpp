#pragma once

#include "command_context.hpp"
#include "icommand.hpp"


class LoadCommand : public ICommand {
public:
    explicit LoadCommand(CommandContext *ctx, QStringList parts)
        : m_ctx(ctx), m_parts(parts) {}
    CommandResult execute() override {
        if (m_parts.length() < 2) {
            return CommandResult::error("Usage: load <file_name>");
        }
        m_ctx->graph->clear();
        m_ctx->graph->beginBatchLoad();
        bool success = m_ctx->repo->load(*m_ctx->graph, m_parts.at(1));
        m_ctx->graph->endBatchLoad();
        if (success) {
            m_ctx->uiContext->store()->setGlobalActiveBox();
            return CommandResult::ok("");
        }
        return CommandResult::error(m_parts.at(1) + " not found");
    }
    void undo() override {}

private:
    CommandContext *m_ctx;
    QStringList m_parts;
};