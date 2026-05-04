#pragma once

struct IPGObserver {
    virtual ~IPGObserver() = default;
    virtual void onNodeAdded(node_id id) {}
    virtual void onEdgeAdded(edge_id id) {}
};