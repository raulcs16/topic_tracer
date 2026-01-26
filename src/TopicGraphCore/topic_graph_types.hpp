#pragma once
#include <cstdint>
#include <string>

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