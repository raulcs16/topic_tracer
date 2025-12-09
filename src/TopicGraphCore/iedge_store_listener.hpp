#pragma once

#include "topic_graph_types.hpp"

struct IEdgeStoreListener {
    virtual ~IEdgeStoreListener() = default;

    virtual void onEdgeAdded(const Edge &edge) {}
    virtual void onEdgeRemoved(const Edge &edge) {}
    virtual void onClear();
};