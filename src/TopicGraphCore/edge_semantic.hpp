#pragma once

#include "topic_graph_types.hpp"

namespace TG::Semantic {
enum class EdgeSemantic {
    Hierarchical,
    Lateral,
    CrossLink
};

static inline EdgeSemantic of(EdgeType type) {
    switch (type) {
    case EdgeType::ComposedOf: return EdgeSemantic::Hierarchical;
    case EdgeType::DependsOn:
    case EdgeType::AlternativeTo: return EdgeSemantic::Lateral;
    case EdgeType::RelatedTo: return EdgeSemantic::CrossLink;
    }
}
} // namespace TG::Semantic