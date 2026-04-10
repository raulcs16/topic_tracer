#pragma once

#include "command_context.hpp"
#include "icommand.hpp"


class SaveCommand : public ICommand {
public:
    explicit SaveCommand(CommandContext *ctx, QStringList parts)
        : m_ctx(ctx), m_parts(parts) {}
    CommandResult execute() override {
        if (m_parts.length() != 2) {
            return CommandResult::error("Usage: save <file_name>");
        }
        auto save = m_ctx->repo->save(*m_ctx->graph, m_parts.at(1));
        if (save) {
            return CommandResult::ok("");
        }
        return CommandResult::error("unable to save");
    }
    void undo() override {}

private:
    CommandContext *m_ctx;
    QStringList m_parts;
};