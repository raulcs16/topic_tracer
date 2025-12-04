#pragma once


#include "icomponent_layout.hpp"
#include "ogdf_strategy.hpp"


class OGDFComponent : public IComponentLayout {

public:
    OGDFComponent(std::shared_ptr<OGDFStrategy> strategy);
    GraphNode &addNode(uint32_t id) override;
    void appendNode(uint32_t id) override;
    void removeNode(uint32_t id) override;
    void addEdge(uint32_t from, uint32_t to) override;
    void removeEdge(uint32_t from, uint32_t to) override;
    void clear() override;
    void apply() override;
    BoundingBox boundingBox() const override;
    void setStrategy(std::shared_ptr<LayoutStrategy> s) override;
    std::vector<GraphNode> nodes() const override { return m_nodes; }
    std::vector<GraphEdge> edges() const override { return m_edges; }

private:
    std::shared_ptr<OGDFContext> m_ogdf;
    std::shared_ptr<OGDFStrategy> m_strategy;
    std::vector<GraphNode> m_nodes;
    std::vector<GraphEdge> m_edges;
    BoundingBox m_bb;
};