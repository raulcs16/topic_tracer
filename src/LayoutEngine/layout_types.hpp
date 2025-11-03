#pragma once

#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/LayoutModule.h>
#include <ogdf/basic/geometry.h>
#include <string>


struct OGDFContext {
    ogdf::Graph graph;
    ogdf::GraphAttributes attributes;
    std::unordered_map<uint32_t, ogdf::node> idToNode;
    std::unordered_map<std::string, ogdf::edge> keyToEdge;
};


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
