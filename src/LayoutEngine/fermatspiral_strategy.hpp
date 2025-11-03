#pragma once
#include "layout_strategy.hpp"


class FermatSpiralStrategy : public LayoutStrategy {
public:
    FermatSpiralStrategy() {};
    void apply(std::vector<GraphNode> &nodes, std::vector<GraphEdge> &edges) override;
};