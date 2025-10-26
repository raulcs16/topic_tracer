#pragma once

#include <ogdf/basic/geometry.h>
#include <string>


struct GraphNode {
    uint32_t id;
    double x;
    double y;
};
struct GraphEdge {
    std::string key;
    uint32_t from;
    uint32_t to;
    double source_x;
    double source_y;
    double target_x;
    double target_y;
    std::vector<ogdf::DPoint> bends;
};
