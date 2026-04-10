#pragma once

#include "graph.hpp"
#include "icommand.hpp"


class UnLinkCommand : public ICommand {
public:
    explicit UnLinkCommand(Graph *graph, QStringList parts)
        : m_graph(graph), m_parts(parts) {}

    CommandResult execute() override {
        if (m_parts.size() != 4) {
            return CommandResult::error("Usage: no link <node_a> <node_b>");
        }
        std::string arg1 = m_parts.at(2).toStdString();
        std::string arg2 = m_parts.at(3).toStdString();
        m_graph->removeEdge(arg1, arg2);
        return CommandResult::ok("");
    }
    void undo() override {}

private:
    Graph *m_graph;
    QStringList m_parts;
};