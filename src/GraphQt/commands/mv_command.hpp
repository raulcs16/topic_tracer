#pragma once

#include "graph.hpp"
#include "icommand.hpp"


class MvCommand : public ICommand {
public:
    explicit MvCommand(Graph *graph, QStringList parts)
        : m_graph(graph), m_parts(parts) {}
    CommandResult execute() override {
        if (m_parts.size() != 3) {
            return CommandResult::error("Usage: mv <node_a> <new_name>");
        }
        m_graph->renameNode(m_parts.at(1).toStdString(), m_parts.at(2).toStdString());
        return CommandResult::ok("");
    }
    void undo() override {}

private:
    Graph *m_graph;
    QStringList m_parts;
};