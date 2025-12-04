#pragma once

#include "icomponent_layout.hpp"
#include <unordered_map>


class PoolComponent : public IComponentLayout {

public:
    PoolComponent(std::shared_ptr<LayoutStrategy> strategy, size_t capacity);

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
    size_t getFreeSlot();

private:
    //allocate size
    struct Slot {
        bool used = false;
        uint32_t id = 0;
    };

    std::vector<GraphNode> m_nodes;               // actual positioned nodes
    std::vector<GraphEdge> m_edges;               // actual positioned nodes
    std::vector<Slot> m_slots;                    // occupancy map
    std::unordered_map<uint32_t, int> m_indexMap; // id → slot index
    size_t m_capacity;

    BoundingBox m_bbox;
    std::shared_ptr<LayoutStrategy> m_strategy;
};