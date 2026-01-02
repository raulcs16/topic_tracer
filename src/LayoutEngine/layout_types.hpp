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
};

struct Camera {
    float screenW = 1300, screenH = 900;
    float zoom = 1.0f;
    float centerX = 0, centerY = 0;

    void project(Transform transform,
                 float localX,
                 float localY,
                 float &sX,
                 float &sY) const {
        float worldX = localX * transform.scale + transform.x;
        float worldY = localY * transform.scale + transform.y;

        float halfViewWidth = (screenW / 2.0f) / zoom;
        float halfViewHeight = (screenH / 2.0f) / zoom;

        float Left = centerX - halfViewWidth;
        float Right = centerX + halfViewWidth;
        float Bottom = centerY - halfViewHeight;
        float Top = centerY + halfViewHeight;

        float ndcX = (worldX - centerX) / halfViewWidth;
        float ndcY = (worldY - centerY) / halfViewHeight;

        sX = (ndcX + 1.0f) * (screenW / 2.0f);
        sY = (ndcY + 1.0f) * (screenH / 2.0f);
    }
};