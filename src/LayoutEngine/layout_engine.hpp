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

    size_t clusterCount() { return m_clusterMap.size(); }
    void enableBatchUpdate(bool enabled = true) { m_batchUpdate = enabled; }

    void addObserver(ILayoutObserver *observer);
    void removeObserver(ILayoutObserver *observer);

    uint32_t getNodeBoundingBox(uint32_t nodeId);
    uint32_t getGlobalBoundingBox();

    //ITopicGraphObserver interface
    void onTopicAdded(const Topic &topic) override;
    void onTopicRemoved(uint32_t id) override;
    void onTopicRenamed(const Topic &topic) override;
    void onEdgeAdded(const Edge &edge) override;
    void onEdgeRemoved(const std::string &key) override;
    void onClear() override;
    void onGraphBluePrint(GraphBlueprint blueprint) override;

private:
    template <typename Func, typename... Args>
    void notify(Func memberFunc, Args &&...args);

private:
    uint32_t nextId() { return m_cluster_id_ref++; }
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
    bool migrate(std::shared_ptr<IClusterLayout> source,
                 std::shared_ptr<IClusterLayout> target);
    void initPool();
    void notifyClusterUpdates(std::shared_ptr<IClusterLayout> cluster);
    void eraseCluster(
        std::unordered_map<uint32_t, std::shared_ptr<IClusterLayout>>::iterator it);

private:
    const uint32_t M_BB_ID = 0;
    const uint32_t M_POOL_ID = 1;
    uint32_t m_cluster_id_ref = 2;
    std::shared_ptr<LayoutStrategy> m_poolStrat;
    std::shared_ptr<OGDFStrategy> m_ogdfStrat;

    //map cluster Id to cluster
    std::unordered_map<uint32_t, std::shared_ptr<IClusterLayout>> m_clusterMap;
    //map nodeId to clusterId
    std::unordered_map<uint32_t, uint32_t> m_nodeToCluster;
    BoundingBox m_global_bb;
    bool m_batchUpdate = false;
    std::vector<ILayoutObserver *> m_observers;
};
