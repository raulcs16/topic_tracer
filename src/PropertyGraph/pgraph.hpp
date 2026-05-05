#pragma once

#include "edge_type_registry.hpp"
#include "node_type_registry.hpp"

using NodeTypeRegPtr = std::shared_ptr<NodeTypeRegistry>;
using EdgeTypeRegPtr = std::shared_ptr<EdgeTypeRegistry>;

class PGraph {
public:
    explicit PGraph(NodeTypeRegPtr node_reg, EdgeTypeRegPtr edge_reg);

    bool addNode(type_id nodeType, const QString &name);
    std::shared_ptr<const Node> getNode(const QString &name);
    bool addEdge(type_id edgeType, node_id from, node_id to);

private:
private:
    NodeTypeRegPtr m_nodeTypeReg;
    EdgeTypeRegPtr m_edgeTypeReg;

    std::unordered_map<node_id, std::shared_ptr<Node>> m_nodes;
    std::unordered_map<edge_id, std::shared_ptr<Edge>> m_edges;
    node_id m_node_id_ref = 0;
    edge_id m_edge_id_ref = 0;
    // std::unordered_map<node_id, std::vector<node_id>> m_adjOutMap;
    // std::unordered_map<node_id, std::vector<node_id>> m_adjInMap;
};