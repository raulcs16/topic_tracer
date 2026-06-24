#pragma once
#include "graph_types.hpp"

using namespace gg;
struct IGraphObserver {
    virtual ~IGraphObserver() = default;

    virtual void onNodeAdded(const Node &node) {}
    virtual void onNodeRemoved(uint32_t id) {}
    virtual void onNodeRenamed(const Node &node) {}
    virtual void onEdgeAdded(const Edge &edge) {}
    virtual void onEdgeRemoved(const std::string &key) {}
    virtual void onClear() {}
    virtual void onGraphBluePrint(GraphBlueprint blueprint) {}
};