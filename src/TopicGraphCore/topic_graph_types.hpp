#pragma once
#include <cstdint>
#include <string>

struct Topic {
    uint32_t id;
    std::string name;
};

enum class EdgeType {
    ComposedOf,    // Concept → Concept
    DependsOn,     // Concrete → Concrete (same cluster)
    AlternativeTo, // Concrete → Concrete (same cluster)
    RelatedTo      // Concept ↔ Concept OR Concrete ↔ Concrete (cross-cluster)
};

struct Edge {
    std::string key;
    uint32_t from;
    uint32_t to;
    EdgeType type;
};