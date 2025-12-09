#pragma once

#include "topic_graph_types.hpp"

struct ITopicStoreListener {
    virtual ~ITopicStoreListener() = default;

    virtual void onTopicAdded(const Topic &topic) {}
    virtual void onTopicRemoved(const Topic &topic) {}
    virtual void onTopicRenamed(const Topic &topic) {}
    virtual void onClear();
};
