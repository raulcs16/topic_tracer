#pragma once

#include "graph.hpp"
#include "icommand.hpp"


class LinkCommand : public ICommand {
public:
    explicit LinkCommand(Graph *graph, QStringList parts)
        : m_graph(graph), m_parts(parts) {}

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
        auto success = m_graph->addEdge(arg1, arg2, edgeType);
        if (success) {
            return CommandResult::ok("");
        }
        return CommandResult::error("unable to link");
    }
    void undo() override {}

private:
    Graph *m_graph;
    QStringList m_parts;
};