#pragma once


#include "layout_types.hpp"


class LayoutStrategy {
public:
    virtual ~LayoutStrategy() = default;
    virtual void apply(std::vector<GraphNode> &nodes, std::vector<GraphEdge> &edge) = 0;
};
