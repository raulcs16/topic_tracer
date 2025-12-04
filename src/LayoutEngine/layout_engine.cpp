#include "fermatspiral_strategy.hpp"
#include "fmmm_strategy.hpp"
#include "graph_keys.hpp"
#include "layout_engine.hpp"
#include "ogdf_component.hpp"


LayoutEngine::LayoutEngine() {
    m_poolStrat = std::make_shared<FermatSpiralStrategy>();
    m_ogdfStrat = std::make_shared<FMMMStrategy>();
    m_pool = std::make_shared<PoolComponent>(m_poolStrat, 100);
}

void LayoutEngine::clear() {}
GraphNode LayoutEngine::addNode(uint32_t id) {
    auto gNode = m_pool->addNode(id);
    m_componetMap[id] = m_pool;
    return gNode;
}
void LayoutEngine::removeNode(uint32_t id) {
    auto it = m_componetMap.find(id);
    if (it != m_componetMap.end()) {
        it->second->removeNode(id);
    }
}
GraphData LayoutEngine::addEdge(uint32_t from, uint32_t to) {
    auto fromIt = m_componetMap.find(from);
    auto toIt = m_componetMap.find(to);
    if (fromIt == m_componetMap.end() || toIt == m_componetMap.end()) {
        throw std::invalid_argument("invalid id");
    }
    std::shared_ptr<IComponentLayout> merger;
    if (fromIt->second == toIt->second) {
        //exist in pool
        if (fromIt->second == m_pool && toIt->second == m_pool) {
            merger = makeComponentFromPool(from, to);
        } else {
            fromIt->second->addEdge(from, to);
            merger = fromIt->second;
        }
    } else if (fromIt->second == m_pool) {
        m_pool->removeNode(from);
        toIt->second->addNode(from);
        toIt->second->addEdge(from, to);
        m_componetMap[from] = toIt->second;
        merger = toIt->second;
    } else if (toIt->second == m_pool) {
        m_pool->removeNode(to);
        fromIt->second->addNode(to);
        fromIt->second->addEdge(from, to);
        m_componetMap[to] = fromIt->second;
        merger = toIt->second;
    } else {
        auto mergeTo =
            fromIt->second->nodes().size() < toIt->second->nodes().size() ? fromIt : toIt;
        auto mergeFrom = mergeTo == fromIt ? toIt : fromIt;

        for (auto &node : mergeFrom->second->nodes()) {
            mergeTo->second->appendNode(node.id);
            m_componetMap[node.id] = mergeTo->second;
        }
        for (auto &edge : mergeFrom->second->edges()) {
            mergeTo->second->addEdge(edge.from, edge.to);
        }
        mergeTo->second->addEdge(from, to);
        mergeTo->second->apply();
        m_componetMap.erase(mergeFrom);
        merger = mergeTo->second;
    }
    return {merger->nodes(), merger->edges()};
}
void LayoutEngine::removeEdge(const std::string &k) {
    auto fromIt = m_componetMap.find(GraphKeys::extractFrom(k));
    auto toIt = m_componetMap.find(GraphKeys::extractTo(k));
    if (fromIt == m_componetMap.end() || toIt == m_componetMap.end()) {
        return;
    }
    if (fromIt != toIt) {
        return;
    }
    fromIt->second->removeEdge(fromIt->first, toIt->first);
}
std::shared_ptr<OGDFComponent> LayoutEngine::makeComponentFromPool(uint32_t from,
                                                                   uint32_t to) {
    auto newComponent = std::make_shared<OGDFComponent>(m_ogdfStrat);
    m_pool->removeNode(from);
    m_pool->removeNode(to);
    newComponent->appendNode(from);
    newComponent->appendNode(to);
    newComponent->addEdge(from, to);
    m_componetMap[from] = newComponent;
    m_componetMap[to] = newComponent;
    return newComponent;
}
