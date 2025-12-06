#pragma once

#include "disjoint_set.hpp"
#include "icluster_layout.hpp"
#include "layout_strategy.hpp"
#include "layout_types.hpp"
#include "ogdf_cluster.hpp"
#include "ogdf_strategy.hpp"
#include "pool_cluster.hpp"


class LayoutEngine {

public:
    explicit LayoutEngine();
    //incremental ops
    GraphNode addNode(uint32_t id);
    void removeNode(uint32_t id);
    GraphData addEdge(uint32_t from, uint32_t to);
    void removeEdge(const std::string &key);
    void clear();

private:
    std::shared_ptr<OGDFCluster> makeClusterFromPool(uint32_t from, uint32_t to);
    inline bool intersects(const BoundingBox &a, const BoundingBox &b) {
        return !(a.max_x < b.min_y || a.min_y > b.max_y || a.max_y < b.min_y ||
                 a.min_y > b.max_y);
    }


private:
    std::shared_ptr<LayoutStrategy> m_poolStrat;
    std::shared_ptr<OGDFStrategy> m_ogdfStrat;
    std::shared_ptr<PoolCluster> m_pool;
    std::unordered_map<uint32_t, std::shared_ptr<IClusterLayout>> m_componetMap;
};
