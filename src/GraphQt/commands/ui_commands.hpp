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
    QString name() const override { return "focus"; }
    QString description() const override { return "focus viewport on a node"; }
    QString usage() const override { return "focus <node_a>"; }

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
    QString name() const override { return "mode"; }
    QString description() const override { return "change the heat mode of the graph"; }
    QString usage() const override { return "mode <stress|progress>"; }

private:
    CommandContext *m_context;
    QStringList m_parts;
};