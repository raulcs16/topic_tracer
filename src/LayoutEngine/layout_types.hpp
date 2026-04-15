#pragma once

#include "geometry.hpp"
#include <string>
#include <vector>


struct GraphNode {
    uint32_t id;
    tt::Point pos;
};
struct GraphEdge {
    std::string key;
    uint32_t from;
    uint32_t to;
    tt::Point source;
    tt::Point target;
    tt::Path bends;
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
