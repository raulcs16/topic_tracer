#pragma once

#include <cstdint>

enum class Edge_Type {
    DependsOn,
    Example,
    ComposedOf,
    AlternativeTo,
    RelatedTo
};

struct Edge {
    uint32_t targetId;
    Edge_Type type;
};