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
};

enum class EdgeType {
    ComposedOf,    // Concept → Concept
    Example,       // Concrete → Concept
    DependsOn,     // Concrete → Concrete
    AlternativeTo, // Concrete → Concrete
    RelatedTo      // Concept ↔ Concept OR Concrete ↔ Concrete (cross-cluster)
};

struct Edge {
    std::string key;
    uint32_t from;
    uint32_t to;
    EdgeType type;
};