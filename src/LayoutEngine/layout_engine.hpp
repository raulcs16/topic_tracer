#pragma once

#include "layout_strategy.hpp"
#include "layout_types.hpp"
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/LayoutModule.h>
#include <ogdf/basic/geometry.h>


class LayoutEngine {

public:
    explicit LayoutEngine();
    void setStrategy(std::unique_ptr<LayoutStrategy> strategy);

    //incremental ops
    void addNode(uint32_t id);
    void removeNode(uint32_t id);
    void addEdge(uint32_t from, uint32_t to);
    void removeEdge(const std::string &key);
    void clear();

    //modify the nodes and edges directly
    void calculateLayout();

    const std::vector<GraphNode> &nodes() const { return m_nodes; }
    const std::vector<GraphEdge> &edges() const { return m_edges; }

private:
    void updateNodesFromAttributes();
    static std::string key(uint32_t from, uint32_t to);
    void assignFermatSpiralPositions();

private:
    ogdf::Graph m_graph;
    ogdf::GraphAttributes m_attributes;
    std::unique_ptr<LayoutStrategy> m_strategy;

    std::unordered_map<uint32_t, ogdf::node> m_idToNode;
    std::unordered_map<std::string, ogdf::edge> m_idToEdge;

    std::vector<GraphNode> m_nodes;
    std::vector<GraphEdge> m_edges;
};
