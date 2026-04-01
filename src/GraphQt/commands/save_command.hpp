#pragma once

#include "command_context.hpp"
#include "icommand.hpp"


class SaveCommand : public ICommand {
public:
    explicit SaveCommand(CommandContext *ctx, QString file) : m_ctx(ctx), m_file(file) {}
    void execute() override { m_ctx->repo->save(*m_ctx->graph, m_file); }
    void undo() override {}

private:
    CommandContext *m_ctx;
    QString m_file;
};