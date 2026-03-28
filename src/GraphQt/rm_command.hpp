#pragma once

#include "graph.hpp"
#include "icommand.hpp"


class RmCommand : public ICommand {
public:
    explicit RmCommand(Graph *graph, QStringList parts)
        : m_graph(graph), m_parts(parts) {}
    void execute() override {
        for (int i = 1; i < m_parts.size(); ++i) {
            QString nodeName = m_parts.at(i);
            m_graph->deleteNode(nodeName.toStdString());
        }
    }
    void undo() override {}

private:
    Graph *m_graph;
    QStringList m_parts;
};