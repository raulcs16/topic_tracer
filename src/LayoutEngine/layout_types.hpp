#pragma once

#include "geometry.hpp"
#include "pgtypes.hpp"
#include <string>
#include <vector>


struct GraphNode {
    node_id id;
    tt::Point pos;
};
struct GraphEdge {
    edge_id id;
    node_id from;
    node_id to;
    tt::Line line;
};

struct GraphData {
    std::vector<GraphNode> nodes;
    std::vector<GraphEdge> edges;
};

struct Transform {
    float x = 0.0f, y = 0.0f;
    float scale = 1.0f;
    float worldX(float localX) { return localX * scale + x; }
    float worldY(float localY) { return localY * scale + y; }
};
