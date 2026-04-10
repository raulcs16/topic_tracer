#pragma once

#include "graph.hpp"
#include "icommand.hpp"


class TouchCommand : public ICommand {
public:
    explicit TouchCommand(Graph *graph, QStringList parts)
        : m_graph(graph), m_parts(parts) {}
    CommandResult execute() override {
        for (int i = 1; i < m_parts.size(); ++i) {
            QString nodeName = m_parts.at(i);
            m_graph->addNode(nodeName.toStdString());
        }
        return CommandResult::ok("");
    }
    void undo() override {}

private:
    Graph *m_graph;
    QStringList m_parts;
};