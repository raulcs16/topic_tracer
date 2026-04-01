#pragma once

#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/LayoutModule.h>
#include <ogdf/basic/geometry.h>
#include <string>


struct BoundingBox {
    double min_x;
    double max_x;
    double min_y;
    double max_y;
};
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

struct ILayoutObserver {
    virtual ~ILayoutObserver() = default;
    virtual void onNodeAdded(const GraphNode &node) {}
    virtual void onNodeUpdated(const GraphNode &node) {}
    virtual void onNodeRemoved(uint32_t id) {}
    virtual void onEdgeAdded(const GraphEdge &edge) {}
    virtual void onEdgeUpdated(const GraphEdge &edge) {}
    virtual void onEdgeRemoved(const std::string &key) {}

    virtual void onClusterRectUpdated(uint32_t clusterId,
                                      float x,
                                      float y,
                                      float w,
                                      float h) {}
    virtual void onClusterRectDeleted(uint32_t) {}
    virtual void onClear() {}
};