#include "fermatspiral_strategy.hpp"
#include "fmmm_strategy.hpp"
#include "graph_keys.hpp"
#include "layout_engine.hpp"
#include "ogdf_cluster.hpp"


LayoutEngine::LayoutEngine() {
    m_poolStrat = std::make_shared<FermatSpiralStrategy>();
    m_ogdfStrat = std::make_shared<FMMMStrategy>();
    m_pool = std::make_shared<PoolCluster>(m_poolStrat, 100);
    m_clusters = 1;
}

void LayoutEngine::clear() {}
void LayoutEngine::addNode(uint32_t id) {
    auto gNode = m_pool->addNode(id);
    m_clusterMap[id] = m_pool;
    notifyNodeAdded(gNode);
}
void LayoutEngine::removeNode(uint32_t id) {
    auto it = m_clusterMap.find(id);
    if (it != m_clusterMap.end()) {
        it->second->removeNode(id);
        notifyNodeRemoved(id);
    }
}
void LayoutEngine::addEdge(uint32_t from, uint32_t to) {
    auto fromIt = m_clusterMap.find(from);
    auto toIt = m_clusterMap.find(to);
    if (fromIt == m_clusterMap.end() || toIt == m_clusterMap.end()) {
        throw std::invalid_argument("invalid id");
    }
    std::shared_ptr<IClusterLayout> merger;
    if (fromIt->second == toIt->second) {
        //exist in pool
        if (fromIt->second == m_pool && toIt->second == m_pool) {
            merger = makeClusterFromPool(from, to);
        } else { //both not in pool but same cluster
            fromIt->second->addEdge(from, to);
            merger = fromIt->second;
        }
        //from or to in pool
    } else if (fromIt->second == m_pool || toIt->second == m_pool) {
        bool fromInPool = fromIt->second == m_pool;
        merger = extractFromPoolMergeNewCluster(from, to, fromInPool);
        //mergeCluster
    } else {
        merger = mergeClusters(from, to);
    }
    if (merger) {
        merger->apply();
    }
}
void LayoutEngine::removeEdge(const std::string &k) {
    auto fromIt = m_clusterMap.find(GraphKeys::extractFrom(k));
    auto toIt = m_clusterMap.find(GraphKeys::extractTo(k));
    if (fromIt == m_clusterMap.end() || toIt == m_clusterMap.end()) {
        return;
    }
    if (fromIt != toIt) {
        return;
    }
    fromIt->second->removeEdge(fromIt->first, toIt->first);
}
std::shared_ptr<OGDFCluster> LayoutEngine::makeClusterFromPool(uint32_t from,
                                                               uint32_t to) {
    auto newCluster = std::make_shared<OGDFCluster>(m_ogdfStrat);
    m_pool->removeNode(from);
    m_pool->removeNode(to);
    newCluster->appendNode(from);
    newCluster->appendNode(to);
    newCluster->addEdge(from, to);
    m_clusterMap[from] = newCluster;
    m_clusterMap[to] = newCluster;
    m_clusters++;
    return newCluster;
}

std::shared_ptr<OGDFCluster> LayoutEngine::mergeClusters(uint32_t from, uint32_t to) {
    auto fromIt = m_clusterMap.find(from);
    auto toIt = m_clusterMap.find(to);
    if (fromIt == m_clusterMap.end() || fromIt->second == m_pool) {
        return nullptr;
    }
    if (toIt == m_clusterMap.end() || toIt->second == m_pool) {
        return nullptr;
    }
    auto newCluster = std::make_shared<OGDFCluster>(m_ogdfStrat);
    auto fromCluster = fromIt->second;
    auto cluster = toIt->second;
    for (auto node : fromCluster->nodes()) {
        newCluster->addNode(node.id);
        m_clusterMap[node.id] = newCluster;
    }
    for (auto node : cluster->nodes()) {
        newCluster->addNode(node.id);
        m_clusterMap[node.id] = newCluster;
    }
    for (auto edge : fromCluster->edges()) {
        newCluster->addEdge(edge.from, edge.to);
    }
    for (auto edge : cluster->edges()) {
        newCluster->addEdge(edge.from, edge.to);
    }
    newCluster->addEdge(from, to);
    m_clusterMap.erase(fromIt);
    m_clusterMap.erase(toIt);
    m_clusters--; //only remove one to account for new one created
    m_clusterMap[from] = newCluster;
    m_clusterMap[to] = newCluster;

    return newCluster;
}
std::shared_ptr<OGDFCluster> LayoutEngine::extractFromPoolMergeNewCluster(
    uint32_t from,
    uint32_t to,
    bool fromInPool) {

    uint32_t pool_id = fromInPool ? from : to;
    uint32_t cluster_id = fromInPool ? to : from;
    auto it = m_clusterMap.find(cluster_id);
    if (it == m_clusterMap.end() || it->second == m_pool) {
        return nullptr;
    }
    m_pool->removeNode(pool_id);
    auto cluster = it->second;
    auto newCluster = std::make_shared<OGDFCluster>(m_ogdfStrat);
    for (auto node : cluster->nodes()) {
        newCluster->addNode(node.id);
        m_clusterMap[node.id] = newCluster;
    }
    for (auto edge : cluster->edges()) {
        newCluster->addEdge(edge.from, edge.to);
    }
    newCluster->addNode(pool_id);
    m_clusterMap[pool_id] = newCluster;
    newCluster->addEdge(from, to);
    return newCluster;
}

void LayoutEngine::onTopicAdded(const Topic &topic) { this->addNode(topic.id); }
void LayoutEngine::onTopicRemoved(const Topic &topic) { this->removeNode(topic.id); }
void LayoutEngine::onTopicRenamed(const Topic &topic) {}
void LayoutEngine::onEdgeAdded(const Edge &edge) { this->addEdge(edge.from, edge.to); }
void LayoutEngine::onEdgeRemoved(const Edge &edge) { this->removeEdge(edge.key); }
void LayoutEngine::onClear() { clear(); }

void LayoutEngine::addObserver(ILayoutObserver *observer) {
    m_observers.push_back(observer);
}

void LayoutEngine::removeObserver(ILayoutObserver *observer) {
    m_observers.erase(
        std::remove_if(m_observers.begin(),
                       m_observers.end(),
                       [observer](ILayoutObserver *it) { return it == observer; }));
}
void LayoutEngine::notifyNodeAdded(const GraphNode &node) {
    // ----- Local space -----
    float localX = node.x;
    float localY = node.y;

    // ----- World space -----
    // Hard-coded world offset
    float worldOffsetX = 500.0f;
    float worldOffsetY = 300.0f;

    float worldX = localX + worldOffsetX;
    float worldY = localY + worldOffsetY;

    // ----- Screen space -----
    // Hard-coded camera + zoom
    float cameraX = 200.0f;
    float cameraY = 100.0f;
    float zoom = 1.25f;

    float screenX = (worldX - cameraX) * zoom;
    float screenY = (worldY - cameraY) * zoom;

    // Copy node so we don’t mutate shared state
    GraphNode screenNode = node;
    screenNode.id = node.id;
    screenNode.x = screenX;
    screenNode.y = screenY;

    for (const auto &obs : m_observers) {
        obs->onNodeAdded(screenNode);
    }
}
void LayoutEngine::notifyNodeRemoved(uint32_t id) {
    for (const auto &obs : m_observers) {
        obs->onNodeRemoved(id);
    }
}
void LayoutEngine::notifyEdgeAdded(const GraphEdge &edge) {
    for (const auto &obs : m_observers) {
        obs->onEdgeAdded(edge);
    }
}
void LayoutEngine::notifyEdgeRemoved(const GraphEdge &edge) {
    for (const auto &obs : m_observers) {
        obs->onEdgeRemoved(edge);
    }
}
void LayoutEngine::notifyClear() {
    for (const auto &obs : m_observers) {
        obs->onClear();
    }
}