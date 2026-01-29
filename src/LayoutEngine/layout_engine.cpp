#include "fermatspiral_strategy.hpp"
#include "fmmm_strategy.hpp"
#include "graph_keys.hpp"
#include "layout_engine.hpp"
#include "ogdf_cluster.hpp"


LayoutEngine::LayoutEngine() {
    m_poolStrat = std::make_shared<FermatSpiralStrategy>();
    m_ogdfStrat = std::make_shared<FMMMStrategy>();
    m_pool = std::make_shared<PoolCluster>(m_poolStrat, 20);
    m_pool->transform().scale = 50;
    m_pool->transform().x = -m_camera.screenW / 4;
    m_clusters.insert(m_pool);
}
LayoutEngine::~LayoutEngine() {
    clear();
    m_observers.clear();
}

void LayoutEngine::clear() {
    m_clusterMap.clear();
    m_pool->clear();
    notify(&ILayoutObserver::onClear);
}
void LayoutEngine::addNode(uint32_t id) {
    auto gNode = m_pool->addNode(id);
    gNode.id = id;
    m_clusterMap[id] = m_pool;
    notify(&ILayoutObserver::onNodeAdded, toScreenNode(gNode, m_pool));
}
void LayoutEngine::removeNode(uint32_t id) {
    auto it = m_clusterMap.find(id);
    if (it != m_clusterMap.end()) {
        it->second->removeNode(id);
        notify(&ILayoutObserver::onNodeRemoved, id);
    }
}
void LayoutEngine::addEdge(uint32_t from, uint32_t to) {
    auto fromIt = m_clusterMap.find(from);
    auto toIt = m_clusterMap.find(to);
    if (fromIt == m_clusterMap.end() || toIt == m_clusterMap.end()) {
        std::cout << "clusterMapNodes:\n";
        for (const auto [node, _] : m_clusterMap) {
            std::cout << "\t" << node << "\n";
        }
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
    if (merger && !m_batchUpdate) {
        merger->apply();
        if (merger != m_pool) {
            resolveCollisions(merger);
        }
        for (auto const &node : merger->nodes()) {
            notify(&ILayoutObserver::onNodeUpdated, toScreenNode(node, merger));
        }
        for (auto const &edge : merger->edges()) {
            notify(&ILayoutObserver::onEdgeAdded, toScreenEdge(edge, merger));
        }
    }
}
void LayoutEngine::applyBatchUpdate() {
    if (m_batchUpdate)
        return;
    for (const auto [_, cluster] : m_clusterMap) {
        if (cluster == m_pool)
            continue;
        cluster->apply();
        resolveCollisions(cluster);
        for (auto const &node : cluster->nodes()) {
            notify(&ILayoutObserver::onNodeUpdated, toScreenNode(node, cluster));
        }
        for (auto const &edge : cluster->edges()) {
            notify(&ILayoutObserver::onEdgeAdded, toScreenEdge(edge, cluster));
        }
    }
}
void LayoutEngine::removeEdge(const std::string &k) {
    auto fromIt = m_clusterMap.find(GraphKeys::extractFrom(k));
    auto toIt = m_clusterMap.find(GraphKeys::extractTo(k));
    if (fromIt == m_clusterMap.end() || toIt == m_clusterMap.end()) {
        return;
    }
    if (fromIt->second != toIt->second) {
        return;
    }
    fromIt->second->removeEdge(fromIt->first, toIt->first);
    notify(&ILayoutObserver::onEdgeRemoved, k);
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
    m_clusters.insert(newCluster);
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
    m_clusters.erase(fromIt->second);
    m_clusterMap.erase(toIt);
    m_clusters.erase(toIt->second);
    m_clusterMap[from] = newCluster;
    m_clusterMap[to] = newCluster;

    m_clusters.insert(newCluster);
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
void LayoutEngine::onTopicRemoved(uint32_t id) { this->removeNode(id); }
void LayoutEngine::onTopicRenamed(const Topic &topic) {}
void LayoutEngine::onEdgeAdded(const Edge &edge) { this->addEdge(edge.from, edge.to); }
void LayoutEngine::onEdgeRemoved(const std::string &key) { this->removeEdge(key); }
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

template <typename Func, typename... Args>
void LayoutEngine::notify(Func memberFunc, Args &&...args) {
    if (m_batchUpdate)
        return;
    for (auto *obs : m_observers) {
        (obs->*memberFunc)(std::forward<Args>(args)...);
    }
}
GraphNode LayoutEngine::toScreenNode(const GraphNode &node,
                                     std::shared_ptr<IClusterLayout> cluster) {
    auto transform = cluster.get()->transform();
    float screenX{0.0f}, screenY{0.0f};
    m_camera.project(transform, node.x, node.y, screenX, screenY);

    GraphNode screenNode = node;
    screenNode.id = node.id;
    screenNode.x = screenX;
    screenNode.y = screenY;
    return screenNode;
}
GraphEdge LayoutEngine::toScreenEdge(const GraphEdge &edge,
                                     std::shared_ptr<IClusterLayout> cluster) {
    auto transform = cluster.get()->transform();
    float targetX{}, targetY{}, sourceX{}, sourceY{};
    m_camera.project(transform, edge.target_x, edge.target_y, targetX, targetY);
    m_camera.project(transform, edge.source_x, edge.source_y, sourceX, sourceY);

    std::vector<ogdf::DPoint> screenbends;
    screenbends.reserve(edge.bends.size());
    for (const auto &point : edge.bends) {
        float x{}, y{};
        m_camera.project(transform, point.m_x, point.m_y, x, y);
        screenbends.emplace_back(x, y);
    }

    GraphEdge screenEdge = edge;
    screenEdge.source_x = sourceX;
    screenEdge.source_y = sourceY;
    screenEdge.target_x = targetX;
    screenEdge.target_y = targetY;
    screenEdge.bends = screenbends;
    return screenEdge;
}

bool LayoutEngine::intersects(std::shared_ptr<IClusterLayout> a,
                              std::shared_ptr<IClusterLayout> b) {
    if (a == b || !a || !b)
        return false;
    auto bbA = a->boundingBox();
    auto transA = a->transform();
    auto bbB = b->boundingBox();
    auto transB = b->transform();


    float aMinX = bbA.min_x * transA.scale + transA.x;
    float aMaxX = bbA.max_x * transA.scale + transA.x;
    float aMinY = bbA.min_y * transA.scale + transA.y;
    float aMaxY = bbA.max_y * transA.scale + transA.y;

    float bMaxX = bbB.max_x * transB.scale + transB.x;
    float bMinX = bbB.min_x * transB.scale + transB.x;
    float bMaxY = bbB.max_y * transB.scale + transB.y;
    float bMinY = bbB.min_y * transB.scale + transB.y;


    return (aMinX <= bMaxX && aMaxX >= bMinX && aMinY <= bMaxY && aMaxY >= bMinY);
}
void LayoutEngine::resolveCollisions(std::shared_ptr<IClusterLayout> newCluster) {
    bool collisionFound = true;

    // We keep looping until we complete a full pass without hitting anything
    while (collisionFound) {
        collisionFound = false;

        // Collect unique clusters (since m_clusterMap has many nodes pointing to same cluster)
        std::set<std::shared_ptr<IClusterLayout>> uniqueClusters;
        for (auto const &[id, cluster] : m_clusterMap) {
            if (cluster != m_pool)
                uniqueClusters.insert(cluster);
        }

        for (auto const &existingCluster : uniqueClusters) {
            if (newCluster == existingCluster)
                continue;

            if (intersects(newCluster, existingCluster)) {
                auto bbNew = newCluster->boundingBox();
                auto bbExt = existingCluster->boundingBox();
                auto transExt = existingCluster->transform();

                // Calculate the Right edge of the cluster we hit in World Space
                float existingRightEdge = (bbExt.max_x * transExt.scale) + transExt.x;

                // Add a small buffer (e.g., 50 units) so they aren't touching pixels
                float buffer = 50.0f;

                // Move newCluster's X so its Left edge starts after existingCluster's Right edge
                // Formula: TargetX = ExistingRightEdge - (NewLocalMinX * NewScale) + Buffer
                float newLocalMinX = bbNew.min_x * newCluster->transform().scale;
                newCluster->transform().x = existingRightEdge - newLocalMinX + buffer;

                collisionFound = true;
                break; // Restart the 'for' loop via the 'while' loop
            }
        }
    }
}
void LayoutEngine::onGraphBluePrint(GraphBlueprint blueprint) {
    clear();
    m_batchUpdate = true;
    //create pool nodes
    for (auto topic : blueprint.isoTopics) {
        addNode(topic.id);
    }
    for (auto cluster : blueprint.clusters) {
        auto newCluster = std::make_shared<OGDFCluster>(m_ogdfStrat);
        m_clusters.insert(newCluster);
        for (auto topic : cluster.topics) {
            newCluster->addNode(topic.id);
            m_clusterMap[topic.id] = newCluster;
        }
        for (auto edge : cluster.edges) {
            newCluster->addEdge(edge.from, edge.to);
        }
        newCluster->apply();
        resolveCollisions(newCluster);
    }
    m_batchUpdate = false;
    for (auto cluster : m_clusters) {
        for (auto const &node : cluster->nodes()) {
            notify(&ILayoutObserver::onNodeAdded, toScreenNode(node, cluster));
        }
        for (auto const &edge : cluster->edges()) {
            notify(&ILayoutObserver::onEdgeAdded, toScreenEdge(edge, cluster));
        }
    }
}