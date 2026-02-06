#pragma once


#include "icluster_layout.hpp"
#include "ogdf_strategy.hpp"


class OGDFCluster : public IClusterLayout {

public:
    OGDFCluster(uint32_t id, std::shared_ptr<OGDFStrategy> strategy);
    uint32_t id() const override { return m_id; }
    void addNode(uint32_t id) override;
    void removeNode(uint32_t id) override;
    void addEdge(uint32_t from, uint32_t to) override;
    void removeEdge(uint32_t from, uint32_t to) override;
    GraphNode *getNode(uint32_t id) const override;
    void clear() override;
    void apply() override;
    BoundingBox boundingBox() const override;
    void setStrategy(std::shared_ptr<LayoutStrategy> s) override;
    std::vector<GraphNode> nodes() const override { return m_nodes; }
    std::vector<GraphEdge> edges() const override { return m_edges; }
    Transform &transform() override { return m_transform; }
    const Transform &transform() const override { return m_transform; }

private:
    uint32_t m_id;
    Transform m_transform;
    std::shared_ptr<OGDFContext> m_ogdf;
    std::shared_ptr<OGDFStrategy> m_strategy;
    std::vector<GraphNode> m_nodes;
    std::vector<GraphEdge> m_edges;
    BoundingBox m_bb;
};