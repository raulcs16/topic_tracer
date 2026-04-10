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

private:
    CommandContext *m_ctx;
    QStringList m_parts;
};