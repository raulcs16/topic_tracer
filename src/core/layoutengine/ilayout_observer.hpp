#pragma once

#include "layout_types.hpp"

struct ILayoutObserver {
    virtual ~ILayoutObserver() = default;
    virtual void onNodeUpdated(const GraphNode &node) {}
    virtual void onNodeClusterChanged(uint32_t nodeId, uint32_t clusterId) {}
    virtual void onEdgeUpdated(const GraphEdge &edge) {}
    virtual void onGlobalBoundsUpdated(tt::Rect rect) {}
    virtual void onClusterRectUpdated(uint32_t clusterId, tt::Rect rect) {}
    virtual void onClusterRectDeleted(uint32_t) {}
};