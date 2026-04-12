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
    QString name() const override { return "load"; }
    QString description() const override { return "load in a file"; }
    QString usage() const override { return "load <file_name>"; }

private:
    CommandContext *m_ctx;
    QStringList m_parts;
};

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
    QString name() const override { return "save"; }
    QString description() const override { return "save current graph state to a file"; }
    QString usage() const override { return "save <file_name>"; }

private:
    CommandContext *m_ctx;
    QStringList m_parts;
};