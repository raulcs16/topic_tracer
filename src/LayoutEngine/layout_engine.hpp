#pragma once

#include "disjoint_set.hpp"
#include "icluster_layout.hpp"
#include "layout_strategy.hpp"
#include "layout_types.hpp"
#include "ogdf_cluster.hpp"
#include "ogdf_strategy.hpp"
#include "pool_cluster.hpp"
#include "topic_graph.hpp"
#include <set>


class LayoutEngine : public ITopicGraphObserver {

public:
    explicit LayoutEngine();
    ~LayoutEngine();
    //incremental ops
    void addNode(uint32_t id);
    void removeNode(uint32_t id);
    void addEdge(uint32_t from, uint32_t to);
    void removeEdge(const std::string &key);
    void clear();
    void applyBatchUpdate();

    size_t clusterCount() { return m_clusters.size(); }


    void addObserver(ILayoutObserver *observer);
    void removeObserver(ILayoutObserver *observer);

    void onTopicAdded(const Topic &topic) override;
    void onTopicRemoved(uint32_t id) override;
    void onTopicRenamed(const Topic &topic) override;
    void onEdgeAdded(const Edge &edge) override;
    void onEdgeRemoved(const std::string &key) override;
    void onClear() override;
    void enableBatchUpdate(bool enabled = true) { m_batchUpdate = enabled; }
    void onGraphBluePrint(GraphBlueprint blueprint) override;

private:
    template <typename Func, typename... Args>
    void notify(Func memberFunc, Args &&...args);

private:
    //both in pool
    std::shared_ptr<OGDFCluster> makeClusterFromPool(uint32_t from, uint32_t to);
    //both not in pool
    std::shared_ptr<OGDFCluster> mergeClusters(uint32_t from, uint32_t to);
    //left in pool,right not
    std::shared_ptr<OGDFCluster> extractFromPoolMergeNewCluster(uint32_t from,
                                                                uint32_t to,
                                                                bool fromInPool);
    bool intersects(std::shared_ptr<IClusterLayout> a, std::shared_ptr<IClusterLayout> b);
    void resolveCollisions(std::shared_ptr<IClusterLayout> newCluster);
    GraphNode toScreenNode(const GraphNode &node, std::shared_ptr<IClusterLayout>);
    GraphEdge toScreenEdge(const GraphEdge &edge, std::shared_ptr<IClusterLayout>);
    void updateGlobalBoundingBox();

private:
    std::shared_ptr<LayoutStrategy> m_poolStrat;
    std::shared_ptr<OGDFStrategy> m_ogdfStrat;

    std::shared_ptr<PoolCluster> m_pool;
    std::unordered_map<uint32_t, std::shared_ptr<IClusterLayout>> m_clusterMap;
    std::set<std::shared_ptr<IClusterLayout>> m_clusters;
    BoundingBox m_global_bb;

    bool m_batchUpdate = false;
    std::vector<ILayoutObserver *> m_observers;
    Camera m_camera;
};
