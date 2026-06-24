#pragma once
#include "pgtypes.hpp"
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/LayoutModule.h>
#include <ogdf/basic/geometry.h>

struct OGDFContext {
    ogdf::Graph graph;
    ogdf::GraphAttributes attributes;
    std::unordered_map<node_id, ogdf::node> idToNode;
    std::unordered_map<edge_id, ogdf::edge> keyToEdge;
};
