#pragma once


#include "icluster_layout.hpp"
// #include "igraph_observer.hpp"
#include "ilayout_observer.hpp"
#include "layout_strategy.hpp"
#include "layout_types.hpp"
#include "ogdf_cluster.hpp"
#include "ogdf_strategy.hpp"
#include "pgtypes.hpp"
#include "pool_cluster.hpp"
#include <set>


using cluster_id = uint32_t;

class LayoutEngine {

public:
    explicit LayoutEngine();
    ~LayoutEngine();
    bool createPool(type_id id);
    //     //incremental ops
    tt::Point addNode(type_id node_type, node_id id);

    cluster_id getClusterIdForType(type_id nodeType) const;
    node_id getNodeIdFromClusterId(cluster_id clusterId) const;
    tt::Rect getClusterRect(cluster_id clusterId) const;

    //     void removeNode(uint32_t id);
    //     void addEdge(uint32_t from, uint32_t to);
    //     void removeEdge(const std::string &key);
    //     void clear();
    //     void applyBatchUpdate();

    //     size_t clusterCount() { return m_clusterMap.size(); }
    //     void enableBatchUpdate(bool enabled = true) { m_batchUpdate = enabled; }

    //     void addObserver(ILayoutObserver *observer);
    //     void removeObserver(ILayoutObserver *observer);

    //     uint32_t getNodeBoundingBox(uint32_t nodeId);
    //     uint32_t getGlobalBoundingBox();

    //     //IGraphObserver interface
    //     void onNodeAdded(const Node &node) override;
    //     void onNodeRemoved(uint32_t id) override;
    //     void onNodeRenamed(const Node &node) override;
    //     void onEdgeAdded(const Edge &edge) override;
    //     void onEdgeRemoved(const std::string &key) override;
    //     void onClear() override;
    //     void onGraphBluePrint(GraphBlueprint blueprint) override;

    // private:
    //     template <typename Func, typename... Args>
    //     void notify(Func memberFunc, Args &&...args);

    //     void mapNodeCluster(uint32_t nodeId, uint32_t clusterId);

private:
    //     uint32_t nextId() { return m_cluster_id_ref++; }
    //     //both in pool
    //     std::shared_ptr<OGDFCluster> makeClusterFromPool(uint32_t from, uint32_t to);
    //     //both not in pool
    //     std::shared_ptr<OGDFCluster> mergeClusters(uint32_t from, uint32_t to);
    //     //left in pool,right not
    //     std::shared_ptr<OGDFCluster> extractFromPoolMergeNewCluster(uint32_t from,
    //                                                                 uint32_t to,
    //                                                                 bool fromInPool);
    bool intersects(std::shared_ptr<IClusterLayout> a, std::shared_ptr<IClusterLayout> b);
    void resolveCollisions(std::shared_ptr<IClusterLayout> newCluster);
    void updateGlobalBoundingBox();
    //     GraphNode toScreenNode(const GraphNode &node, std::shared_ptr<IClusterLayout>);
    //     GraphEdge toScreenEdge(const GraphEdge &edge, std::shared_ptr<IClusterLayout>);
    //     bool migrate(std::shared_ptr<IClusterLayout> source,
    //                  std::shared_ptr<IClusterLayout> target);
    //     void initPool();
    //     void notifyClusterUpdates(std::shared_ptr<IClusterLayout> cluster);
    //     void eraseCluster(
    //         std::unordered_map<uint32_t, std::shared_ptr<IClusterLayout>>::iterator it);

private:
    std::shared_ptr<LayoutStrategy> m_poolStrat;
    std::shared_ptr<OGDFStrategy> m_ogdfStrat;
    cluster_id m_cluster_id_ref = 1;
    //map cluster Id to cluster
    std::unordered_map<cluster_id, std::shared_ptr<IClusterLayout>> m_clusterMap;
    std::unordered_map<type_id, cluster_id> m_nodeTypeToClusterId;
    //map nodeId to clusterId
    std::unordered_map<node_id, cluster_id> m_nodeToCluster;
    tt::BoundingBox m_global_bb;
    //     bool m_batchUpdate = false;
    //     std::vector<ILayoutObserver *> m_observers;
    //     const uint32_t M_BB_ID = 0;
    //     const uint32_t M_POOL_ID = 1;
};
