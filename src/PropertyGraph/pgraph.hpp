#pragma once

#include "edge_registry.hpp"
#include "node_registry.hpp"

using NodeRegPtr = std::shared_ptr<NodeRegistry>;
using EdgeRegPtr = std::shared_ptr<EdgeRegistry>;

class PGraph {
public:
    explicit PGraph(NodeRegPtr node_reg, EdgeRegPtr edge_reg);

    bool addNode(type_id nodeType, const QString &name);
    std::shared_ptr<const Node> getNode(const QString &name);
    bool addEdge(type_id edgeType, node_id from, node_id to);

private:
    NodeRegPtr m_node_reg;
    EdgeRegPtr m_edge_reg;
    std::unordered_map<node_id, std::shared_ptr<Node>> m_nodes;
    std::unordered_map<edge_id, std::shared_ptr<Edge>> m_edges;
    node_id m_node_id_ref = 0;
    edge_id m_edge_id_ref = 0;
    // std::unordered_map<node_id, std::vector<node_id>> m_adjOutMap;
    // std::unordered_map<node_id, std::vector<node_id>> m_adjInMap;
};