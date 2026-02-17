#pragma once

#include "graph_types.hpp"

namespace TG::Semantic {
enum class EdgeSemantic {
    RoadMap,
    Architectural
};

static inline EdgeSemantic of(EdgeType type) {
    switch (type) {
    case EdgeType::ComposedOf:
    case EdgeType::DependsOn:
    case EdgeType::AlternativeTo:
    case EdgeType::RelatedTo: return EdgeSemantic::RoadMap;
    case EdgeType::Import:
    case EdgeType::Inject:
    case EdgeType::Implements: return EdgeSemantic::Architectural;
    }
}
} // namespace TG::Semantic