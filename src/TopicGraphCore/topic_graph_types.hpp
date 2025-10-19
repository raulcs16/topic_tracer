#pragma once
#include "graph_types.hpp"
#include <cstdint>
#include <string>

enum class Topic_Type {
    Concept,
    Concrete
};

struct Topic {
    uint32_t id;
    std::string name;
    bool covered;
    Topic_Type type;
};

enum class Edge_Type {
    DependsOn,
    Example,
    ComposedOf,
    AlternativeTo,
    RelatedTo
};


struct TopicNode {
    Topic topic;
    GraphNode geometry;
};
struct TopicEdge {
    uint32_t from;
    uint32_t to;
    Edge_Type relation;
    GraphEdge geometry;
};