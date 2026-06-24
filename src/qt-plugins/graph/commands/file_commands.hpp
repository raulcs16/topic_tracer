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
    QString getHint() const override { return usage(); }

    std::unique_ptr<ICommand> clone(const QStringList &parts) const override {
        return std::make_unique<LoadCommand>(m_ctx, parts);
    }

    QStringList getValidArgs(const QStringList &parts) const override {
        auto files = m_ctx->fileManger->listFiles();
        if (parts.size() <= 1) {
            return files;
        }
        QStringList results;
        for (const auto &file : files) {
            if (file.startsWith(parts.last(), Qt::CaseSensitive)) {
                results.append(file);
            }
        }
        return results;
    }

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
    QString getHint() const override { return usage(); }

    std::unique_ptr<ICommand> clone(const QStringList &parts) const override {
        return std::make_unique<SaveCommand>(m_ctx, parts);
    }

    QStringList getValidArgs(const QStringList &parts) const override {
        QStringList results;
        return results;
    }

private:
    CommandContext *m_ctx;
    QStringList m_parts;
};

class LsCommand : public ICommand {
public:
    explicit LsCommand(CommandContext *ctx, QStringList parts)
        : m_ctx(ctx), m_parts(parts) {}
    CommandResult execute() override {
        auto files = m_ctx->fileManger->listFiles();
        if (files.size() > 0) {
            return CommandResult::ok(files.join("\t"));
        }
        return CommandResult::error("unable to find files @" +
                                    m_ctx->fileManger->getBaseDataPath());
    }
    void undo() override {}
    QString name() const override { return "ls"; }
    QString description() const override { return "list all files"; }
    QString usage() const override { return "ls"; }
    QString getHint() const override { return usage(); }

    std::unique_ptr<ICommand> clone(const QStringList &parts) const override {
        return std::make_unique<LsCommand>(m_ctx, parts);
    }

    QStringList getValidArgs(const QStringList &parts) const override {
        QStringList results;
        return results;
    }

private:
    CommandContext *m_ctx;
    QStringList m_parts;
};

class PWDCommand : public ICommand {
public:
    explicit PWDCommand(CommandContext *ctx, QStringList parts)
        : m_ctx(ctx), m_parts(parts) {}
    CommandResult execute() override {
        auto path = m_ctx->fileManger->getBaseDataPath();
        return CommandResult::ok(path);
    }
    void undo() override {}
    QString name() const override { return "pwd"; }
    QString description() const override { return "list directory of files"; }
    QString usage() const override { return "pwd"; }
    QString getHint() const override { return usage(); }

    std::unique_ptr<ICommand> clone(const QStringList &parts) const override {
        return std::make_unique<PWDCommand>(m_ctx, parts);
    }

    QStringList getValidArgs(const QStringList &parts) const override {
        QStringList results;
        return results;
    }

private:
    CommandContext *m_ctx;
    QStringList m_parts;
};
