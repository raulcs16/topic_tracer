#pragma once
#include <cstdint>
#include <string>

enum class Topic_Type {
    Concept,
    Concrete
};

struct Topic {
    uint32_t id;
    std::string name;
    Topic_Type type;
    bool covered;
};

enum class Edge_Type {
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
    Edge_Type type;
    bool directed = true;
};