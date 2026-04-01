#pragma once

#include "command_context.hpp"
#include "icommand.hpp"


class LoadCommand : public ICommand {
public:
    explicit LoadCommand(CommandContext *ctx, QString file) : m_ctx(ctx), m_file(file) {}
    void execute() override {
        m_ctx->graph->clear();
        m_ctx->graph->beginBatchLoad();
        bool success = m_ctx->repo->load(*m_ctx->graph, m_file);
        m_ctx->graph->endBatchLoad();
        if (success) {
            m_ctx->uiContext->store()->setGlobalActiveBox();
        }
    }
    void undo() override {}

private:
    CommandContext *m_ctx;
    QString m_file;
};