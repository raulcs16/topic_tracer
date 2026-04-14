#pragma once
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/LayoutModule.h>
#include <ogdf/basic/geometry.h>

struct OGDFContext {
    ogdf::Graph graph;
    ogdf::GraphAttributes attributes;
    std::unordered_map<uint32_t, ogdf::node> idToNode;
    std::unordered_map<std::string, ogdf::edge> keyToEdge;
};
