#pragma once

#include "command_context.hpp"
#include "icommand.hpp"
#include "path_analyzer.hpp"


class PathCommand : public ICommand {
public:
    explicit PathCommand(CommandContext *ctx, QStringList parts)
        : m_ctx(ctx), m_parts(parts) {}
    CommandResult execute() override {
        if (m_parts.size() != 3) {
            return CommandResult::error("Usage: path <node_a> <node_b>");
        }
        auto ta = m_ctx->graph->getNode(m_parts.at(1).toStdString());
        auto tb = m_ctx->graph->getNode(m_parts.at(2).toStdString());
        if (ta == nullptr || tb == nullptr) {
            return CommandResult::error(m_parts.at(1) + " & or " + m_parts.at(2) +
                                        " not found");
        }
        auto parents = TG::PathAnalyzer::dijsktras(*m_ctx->graph, ta->id, tb->id);
        auto topicIds = TG::PathAnalyzer::topicPath(parents, tb->id);

        std::unordered_set<int> topicSet(topicIds.begin(), topicIds.end());
        for (const auto &topic : m_ctx->graph->nodes()) {
            if (topicSet.contains(topic->id)) {
                m_ctx->uiContext->store()->setNodeState(topic->id,
                                                        StateFlag::InPath,
                                                        true);
                m_ctx->uiContext->store()->setNodeState(topic->id,
                                                        StateFlag::Hidden,
                                                        false);
            } else {
                m_ctx->uiContext->store()->setNodeState(topic->id,
                                                        StateFlag::InPath,
                                                        false);
                m_ctx->uiContext->store()->setNodeState(topic->id,
                                                        StateFlag::Hidden,
                                                        true);
            }
        }


        auto edgeKeys = TG::PathAnalyzer::edgePath(topicIds);
        std::unordered_set<std::string> edgeSet(edgeKeys.begin(), edgeKeys.end());
        for (const auto &edge : m_ctx->graph->edges()) {
            if (edgeSet.contains(edge->key)) {
                m_ctx->uiContext->store()->setEdgeState(edge->key,
                                                        StateFlag::InPath,
                                                        true);
                m_ctx->uiContext->store()->setEdgeState(edge->key,
                                                        StateFlag::Hidden,
                                                        false);
            } else {
                m_ctx->uiContext->store()->setEdgeState(edge->key,
                                                        StateFlag::InPath,
                                                        false);
                m_ctx->uiContext->store()->setEdgeState(edge->key,
                                                        StateFlag::Hidden,
                                                        true);
            }
        }
        return CommandResult::ok("");
    }
    void undo() override {}

private:
    CommandContext *m_ctx;
    QStringList m_parts;
};