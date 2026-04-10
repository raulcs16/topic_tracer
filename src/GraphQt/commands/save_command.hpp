#pragma once

#include "command_context.hpp"
#include "icommand.hpp"


class SaveCommand : public ICommand {
public:
    explicit SaveCommand(CommandContext *ctx, QString file) : m_ctx(ctx), m_file(file) {}
    CommandResult execute() override {
        auto save = m_ctx->repo->save(*m_ctx->graph, m_file);
        if (save) {
            return CommandResult::ok("");
        }
        return CommandResult::error("unable to save");
    }
    void undo() override {}

private:
    CommandContext *m_ctx;
    QString m_file;
};