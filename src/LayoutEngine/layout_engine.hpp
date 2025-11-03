#pragma once

#include "layout_strategy.hpp"
#include "layout_types.hpp"


class LayoutEngine {

public:
    explicit LayoutEngine();
    void setStrategy(std::unique_ptr<LayoutStrategy> strategy);

    inline OGDFContext &ogdfContext() { return m_ogdf; }

    //incremental ops
    void addNode(uint32_t id);
    void removeNode(uint32_t id);
    void addEdge(uint32_t from, uint32_t to);
    void removeEdge(const std::string &key);
    void clear();

    void calculateLayout();

    inline const std::vector<GraphNode> &nodes() const { return m_nodes; }
    inline const std::vector<GraphEdge> &edges() const { return m_edges; }

private:
    static std::string key(uint32_t from, uint32_t to);

private:
    OGDFContext m_ogdf;

    std::unique_ptr<LayoutStrategy> m_strategy;
    std::vector<GraphNode> m_nodes;
    std::vector<GraphEdge> m_edges;
};
