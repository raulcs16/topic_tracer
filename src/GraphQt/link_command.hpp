#pragma once

#include "graph.hpp"
#include "icommand.hpp"


class LinkCommand : public ICommand {
public:
    explicit LinkCommand(Graph *graph, QStringList parts)
        : m_graph(graph), m_parts(parts) {}

    void execute() override {
        if (m_parts.size() < 3)
            return;
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
        m_graph->addEdge(arg1, arg2, edgeType);
    }
    void undo() override {}

private:
    Graph *m_graph;
    QStringList m_parts;
};