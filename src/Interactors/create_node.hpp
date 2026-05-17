#pragma once

#include "io_pgraph.hpp"
#include "layout_engine.hpp"
#include "p"
#include "pgraph.hpp"


struct CreateNodeResponse {
    bool success;
    QString error;
    node_id id;
};
class CreateNode {
public:
    CreateNode(PGraph &graph, LayoutEngine &layout, IOPGraph &io)
        : m_io{io}, m_graph{graph}, m_layout{layout} {}
    CreateNodeResponse execute(const QString &typeSymbol, const QString &name) {
        auto type = m_graph.nodeTypes()->get(typeSymbol);
        if (type == nullptr) {
            return {false, "error:unable to create due to type not found:", typeSymbol};
        }
        auto node = m_graph.addNode(type->typeId, name);
        if (node == nullptr) {
            return {false, "error:unable to create: " + name, 0};
        }
        auto pos = m_layout.addNode(node->typeId, node->id);
        m_graph.updateNodePosition(node->id, pos);
        m_io.onNodeCreated(node->id);
        return {true, "successfully created!", node->id};
    }

private:
    PGraph &m_graph;
    LayoutEngine &m_layout;
    IOPGraph &m_io;
};