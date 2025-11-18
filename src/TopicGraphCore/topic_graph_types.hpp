#pragma once
#include <cstdint>
#include <string>

enum class TopicType {
    Concept,
    Concrete
};

struct Topic {
    uint32_t id;
    std::string name;
    TopicType type;
    bool covered;
};

enum class EdgeType {
    DependsOn,
    Example,
    ComposedOf,
    AlternativeTo,
    RelatedTo
};

struct Edge {
    std::string key;
    uint32_t from;
    uint32_t to;
    EdgeType type;
    bool directed = true;
};