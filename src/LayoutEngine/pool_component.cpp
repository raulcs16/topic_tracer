#include "pool_component.hpp"

PoolComponent::PoolComponent(std::shared_ptr<LayoutStrategy> strategy, size_t capacity)
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

GraphNode &PoolComponent::addNode(uint32_t id) {
    size_t slot = getFreeSlot();
    m_slots[slot].used = true;
    m_slots[slot].id = id;
    m_indexMap[id] = slot;

    auto &node = m_nodes[slot];
    node.id = id;
    return node;
}
void PoolComponent::appendNode(uint32_t id) { addNode(id); }


//WARNING: currently returns last slot position if none available
size_t PoolComponent::getFreeSlot() {
    size_t i = 0;
    for (; i < m_slots.size(); i++) {
        if (!m_slots[i].used)
            return i;
    }
    return i--;
}
void PoolComponent::removeNode(uint32_t id) {
    auto it = m_indexMap.find(id);
    if (it == m_indexMap.end())
        return;
    size_t slot = it->second;
    m_slots[slot].id = 0;
    m_slots[slot].used = false;
    m_indexMap.erase(it);
}
void PoolComponent::addEdge(uint32_t from, uint32_t to) {}
void PoolComponent::removeEdge(uint32_t from, uint32_t to) {}
void PoolComponent::clear() {}
void PoolComponent::apply() {
    if (m_strategy) {
        std::vector<GraphEdge> edges;
        m_strategy->apply(m_nodes, edges);
    }
}
BoundingBox PoolComponent::boundingBox() const { return m_bbox; }
void PoolComponent::setStrategy(std::shared_ptr<LayoutStrategy> s) { m_strategy = s; }
