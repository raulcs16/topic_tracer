#pragma once

#include "icluster_layout.hpp"
#include <unordered_map>


class PoolCluster : public IClusterLayout {

public:
    PoolCluster(uint32_t id, std::shared_ptr<LayoutStrategy> strategy, size_t capacity);
    uint32_t id() const override { return m_id; }
    void addNode(node_id id) override;
    void removeNode(node_id id) override;
    GraphNode *getNode(node_id id) const override;
    void addEdge(node_id from, node_id to, edge_id id) override {}
    void removeEdge(edge_id id) override {}
    void clear() override;
    void apply() override;
    tt::BoundingBox boundingBox() const override;
    void setStrategy(std::shared_ptr<LayoutStrategy> s) override;
    std::vector<GraphNode> nodes() const override;
    std::vector<GraphEdge> edges() const override { return m_edges; }
    Transform &transform() override { return m_transfrom; }
    const Transform &transform() const override { return m_transfrom; }

private:
    size_t getFreeSlot();

private:
    uint32_t m_id;
    //allocate size
    struct Slot {
        bool used = false;
        uint32_t id = 0;
    };

    Transform m_transfrom;
    std::vector<GraphNode> m_nodes; // actual positioned nodes
    std::vector<GraphEdge> m_edges; // actual positioned edges
    tt::BoundingBox m_bbox;
    std::vector<Slot> m_slots;                    // occupancy map
    std::unordered_map<uint32_t, int> m_indexMap; // id → slot index
    size_t m_capacity;

    std::shared_ptr<LayoutStrategy> m_strategy;
};