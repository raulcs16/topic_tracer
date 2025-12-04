#pragma once

#include "disjoint_set.hpp"
#include "icomponent_layout.hpp"
#include "layout_strategy.hpp"
#include "layout_types.hpp"
#include "ogdf_component.hpp"
#include "ogdf_strategy.hpp"
#include "pool_component.hpp"


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
    std::shared_ptr<OGDFComponent> makeComponentFromPool(uint32_t from, uint32_t to);

private:
    std::shared_ptr<LayoutStrategy> m_poolStrat;
    std::shared_ptr<OGDFStrategy> m_ogdfStrat;
    std::shared_ptr<PoolComponent> m_pool;
    std::unordered_map<uint32_t, std::shared_ptr<IComponentLayout>> m_componetMap;
};
