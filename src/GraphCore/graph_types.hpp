#pragma once
#include <cstdint>
#include <string>
#include <vector>

enum class EdgeType {
    Null,
    Composes,
    Associates,
    Aggregates,
    Injects,
    Implements,
};

struct Node {
    uint32_t id;
    std::string label;
};
struct Edge {
    std::string key;
    uint32_t from;
    uint32_t to;
    EdgeType type;
};

struct SemanticCluster {
    std::vector<Node> nodes;
    std::vector<Edge> edges;
};

struct GraphBlueprint {
    std::vector<SemanticCluster> clusters;
    std::vector<Node> isoNodes;
};