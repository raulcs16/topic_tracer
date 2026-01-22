#include "pool_cluster.hpp"

PoolCluster::PoolCluster(std::shared_ptr<LayoutStrategy> strategy, size_t capacity)
    : m_strategy{strategy} {
    m_capacity = capacity;
    m_nodes.reserve(capacity);
    m_slots.reserve(capacity);
    m_indexMap.reserve(capacity);
    for (size_t i = 0; i < capacity; i++) {
        m_nodes.emplace_back(0, 0, 0);
        m_slots.emplace_back(false, 0);
    }
    apply();
}

GraphNode &PoolCluster::addNode(uint32_t id) {
    size_t slot = getFreeSlot();
    m_slots[slot].used = true;
    m_slots[slot].id = id;
    m_indexMap[id] = slot;

    auto &node = m_nodes[slot];
    node.id = id;
    return node;
}
void PoolCluster::appendNode(uint32_t id) { addNode(id); }


size_t PoolCluster::getFreeSlot() {
    size_t i = 0;
    for (; i < m_slots.size(); i++) {
        if (!m_slots[i].used)
            return i;
    }
    //allocate bigger pool:
    size_t old_cap = m_capacity;
    m_capacity *= 4;
    std::vector<GraphEdge> edges;
    std::vector<GraphNode> nodes;
    for (size_t i = 0; i < m_capacity; i++) {
        nodes.emplace_back(0, 0, 0);
        if (i >= old_cap) {
            m_slots.push_back({false, 0});
        }
    }
    m_strategy->apply(nodes, edges, m_bbox);
    for (size_t i = old_cap; i < m_capacity; i++) {
        m_nodes.push_back(nodes[i]);
    }
    return m_capacity;
}
void PoolCluster::removeNode(uint32_t id) {
    auto it = m_indexMap.find(id);
    if (it == m_indexMap.end())
        return;
    size_t slot = it->second;
    m_slots[slot].id = 0;
    m_slots[slot].used = false;
    m_indexMap.erase(it);
}
void PoolCluster::addEdge(uint32_t from, uint32_t to) {}
void PoolCluster::removeEdge(uint32_t from, uint32_t to) {}
void PoolCluster::clear() {}
void PoolCluster::apply() {
    if (m_strategy) {
        std::vector<GraphEdge> edges;
        m_strategy->apply(m_nodes, edges, m_bbox);
    }
}
BoundingBox PoolCluster::boundingBox() const { return m_bbox; }
void PoolCluster::setStrategy(std::shared_ptr<LayoutStrategy> s) { m_strategy = s; }
