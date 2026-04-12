#pragma once

#include "command_context.hpp"
#include "icommand.hpp"


class ClearCommand : public ICommand {
public:
    explicit ClearCommand(CommandContext *context, QStringList parts)
        : m_context(context), m_parts(parts) {}

    CommandResult execute() override {
        m_context->graph->clear();
        return CommandResult::ok("");
    }
    void undo() override {}
    QString name() const override { return "clear"; }
    QString description() const override { return "clear current graph state"; }
    QString usage() const override { return "clear"; }

private:
    CommandContext *m_context;
    QStringList m_parts;
};

class ClearPathCommand : public ICommand {
public:
    explicit ClearPathCommand(CommandContext *context, QStringList parts)
        : m_context(context), m_parts(parts) {}
    CommandResult execute() override {
        m_context->uiContext->store()->clearPath();
        return CommandResult::ok("");
    }
    void undo() override {}
    QString name() const override { return "no path"; }
    QString description() const override {
        return "un highlight the current highlighted path";
    }
    QString usage() const override { return "no path"; }

private:
    CommandContext *m_context;
    QStringList m_parts;
};

class LinkCommand : public ICommand {
public:
    explicit LinkCommand(CommandContext *context, QStringList parts)
        : m_context(context), m_parts(parts) {}
    CommandResult execute() override {
        if (m_parts.size() < 3) {

            return CommandResult::error("Usage: link <nodeA> <nodeB> -t <link-type>");
        }
        std::string arg1 = m_parts.at(1).toStdString();
        std::string arg2 = m_parts.at(2).toStdString();
        EdgeType edgeType = EdgeType::Composes;
        if (m_parts.size() == 5) {
            QString type = m_parts.last().toLower();
            if (type == "associates")
                edgeType = EdgeType::Associates;
            if (type == "aggregates")
                edgeType = EdgeType::Aggregates;
            if (type == "injects")
                edgeType = EdgeType::Injects;
            if (type == "implements")
                edgeType = EdgeType::Implements;
        }
        auto success = m_context->graph->addEdge(arg1, arg2, edgeType);
        if (success) {
            return CommandResult::ok("");
        }
        return CommandResult::error("unable to link");
    }
    void undo() override {}
    QString name() const override { return "link"; }
    QString description() const override { return "link two nodes together"; }
    QString usage() const override {
        return "link <node_a> <node_b> -t <composses | injects | implements | associates "
               " | aggregates>";
    }

private:
    CommandContext *m_context;
    QStringList m_parts;
};

class MvCommand : public ICommand {
public:
    explicit MvCommand(CommandContext *context, QStringList parts)
        : m_context(context), m_parts(parts) {}
    CommandResult execute() override {
        if (m_parts.size() != 3) {
            return CommandResult::error("Usage: mv <node_a> <new_name>");
        }
        m_context->graph->renameNode(m_parts.at(1).toStdString(),
                                     m_parts.at(2).toStdString());
        return CommandResult::ok("");
    }
    void undo() override {}
    QString name() const override { return "mv"; }
    QString description() const override { return "rename a node"; }
    QString usage() const override { return "mv  <node_a> <new_name>"; }

private:
    CommandContext *m_context;
    QStringList m_parts;
};

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
    QString name() const override { return "path"; }
    QString description() const override {
        return "highlight the path between starting node and ending node";
    }
    QString usage() const override { return "path <node_a> <node_b>"; }

private:
    CommandContext *m_ctx;
    QStringList m_parts;
};

class RmCommand : public ICommand {
public:
    explicit RmCommand(CommandContext *context, QStringList parts)
        : m_context(context), m_parts(parts) {}
    CommandResult execute() override {
        for (int i = 1; i < m_parts.size(); ++i) {
            QString nodeName = m_parts.at(i);
            m_context->graph->deleteNode(nodeName.toStdString());
        }
        return CommandResult::ok("");
    }
    void undo() override {}
    QString name() const override { return "rm"; }
    QString description() const override {
        return "remove an existing node from the graph";
    }
    QString usage() const override { return "rm <node_a>"; }

private:
    CommandContext *m_context;
    QStringList m_parts;
};

class TouchCommand : public ICommand {
public:
    explicit TouchCommand(CommandContext *context, QStringList parts)
        : m_context(context), m_parts(parts) {}
    CommandResult execute() override {
        for (int i = 1; i < m_parts.size(); ++i) {
            QString nodeName = m_parts.at(i);
            m_context->graph->addNode(nodeName.toStdString());
        }
        return CommandResult::ok("");
    }
    void undo() override {}
    QString name() const override { return "touch"; }
    QString description() const override { return "create one or multiple nodes"; }
    QString usage() const override { return "touch <node_a> <node_b> ... <node_n>"; }

private:
    CommandContext *m_context;
    QStringList m_parts;
};

class UnLinkCommand : public ICommand {
public:
    explicit UnLinkCommand(CommandContext *context, QStringList parts)
        : m_context(context), m_parts(parts) {}

    CommandResult execute() override {
        if (m_parts.size() != 4) {
            return CommandResult::error("Usage: no link <node_a> <node_b>");
        }
        std::string arg1 = m_parts.at(2).toStdString();
        std::string arg2 = m_parts.at(3).toStdString();
        m_context->graph->removeEdge(arg1, arg2);
        return CommandResult::ok("");
    }
    void undo() override {}
    QString name() const override { return "no link"; }
    QString description() const override { return "remove the link between two nodes"; }
    QString usage() const override { return "no link <node_a> <node_b>"; }

private:
    CommandContext *m_context;
    QStringList m_parts;
};