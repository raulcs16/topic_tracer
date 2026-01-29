#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct Topic {
    uint32_t id;
    std::string name;
};

enum class EdgeType {
    ComposedOf,
    DependsOn,
    AlternativeTo,
    RelatedTo
};

struct Edge {
    std::string key;
    uint32_t from;
    uint32_t to;
    EdgeType type;
};

struct SemanticCluster {
    std::vector<Topic> topics;
    std::vector<Edge> edges;
};

struct GraphBlueprint {
    std::vector<SemanticCluster> clusters;
    std::vector<Topic> isoTopics;
};