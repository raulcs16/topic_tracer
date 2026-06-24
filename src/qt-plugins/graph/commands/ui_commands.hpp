#pragma once

#include "command_context.hpp"
#include "icommand.hpp"


class FocusCommand : public ICommand {
public:
    explicit FocusCommand(CommandContext *ctx, QStringList parts)
        : m_ctx(ctx), m_parts(parts) {}
    CommandResult execute() override {
        if (m_parts.size() != 2) {

            return CommandResult::error("Usage: focus <node_name>");
        }

        auto topic = m_ctx->graph->getNode(m_parts.at(1).toStdString());
        if (topic != nullptr) {
            auto clusterId = m_ctx->layout->getNodeBoundingBox(topic->id);
            return CommandResult::ok("");
        }
        return CommandResult::error(m_parts.at(1) + " not found");
    }
    void undo() override {}
    QString getHint() const override { return ""; }
    QString name() const override { return "focus"; }
    QString description() const override { return "focus viewport on a node"; }
    QString usage() const override { return "focus <node_a>"; }


    std::unique_ptr<ICommand> clone(const QStringList &parts) const override {
        return std::make_unique<FocusCommand>(m_ctx, parts);
    }

    QStringList getValidArgs(const QStringList &parts) const override {
        QStringList results;
        return results;
    }

private:
    CommandContext *m_ctx;
    QStringList m_parts;
};


class ModeCommand : public ICommand {
public:
    explicit ModeCommand(CommandContext *context, QStringList parts)
        : m_context(context), m_parts(parts) {}
    CommandResult execute() override {
        if (m_parts.length() != 2) {
            return CommandResult::error("Usage: mode <'progress|stress'>");
        }
        if (m_parts.at(1).toLower() == "progress") {
            m_context->uiContext->setMode(UIContext::ViewMode::Progress);
            return CommandResult::ok("");
        } else if (m_parts.at(1).toLower() == "stress") {
            m_context->uiContext->setMode(UIContext::ViewMode::Stress);
            return CommandResult::ok("");
        }
        return CommandResult::error("Usage: mode <'progress|stress'>");
    }
    void undo() override {}
    QString getHint() const override { return ""; }
    QString name() const override { return "mode"; }
    QString description() const override { return "change the heat mode of the graph"; }
    QString usage() const override { return "mode <stress|progress>"; }


    std::unique_ptr<ICommand> clone(const QStringList &parts) const override {
        return std::make_unique<ModeCommand>(m_context, parts);
    }

    QStringList getValidArgs(const QStringList &parts) const override {
        QStringList results;
        return results;
    }

private:
    CommandContext *m_context;
    QStringList m_parts;
};

class HelpCommand : public ICommand {
public:
    explicit HelpCommand(std::map<QString, std::unique_ptr<ICommand>> &commands)
        : m_commands(commands) {}
    CommandResult execute() override {
        CommandResult result;
        result.type = EntryType::Output;
        result.success = true;

        QStringList list;
        for (const auto &[name, cmd] : m_commands) {
            auto info = name + ":\t" + cmd->description();
            list.push_back(info);
        }
        result.message = list.join('\n');


        return result;
    }
    void undo() override {}
    QString getHint() const override { return ""; }
    QString name() const override { return "help"; }
    QString description() const override {
        return "view list of commands, and their usage";
    }
    QString usage() const override { return "help"; }


    std::unique_ptr<ICommand> clone(const QStringList &parts) const override {
        return std::make_unique<HelpCommand>(m_commands);
    }

    QStringList getValidArgs(const QStringList &parts) const override {
        QStringList results;
        return results;
    }

private:
    std::map<QString, std::unique_ptr<ICommand>> &m_commands;
};