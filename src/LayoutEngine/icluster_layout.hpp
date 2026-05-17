#pragma once

#include "geometry.hpp"
#include "layout_strategy.hpp"
#include "layout_types.hpp"
#include "pgtypes.hpp"
#include <cstdint>
#include <string>


class IClusterLayout {
public:
    virtual uint32_t id() const = 0;
    virtual void addNode(node_id id) = 0;
    virtual void removeNode(node_id id) = 0;
    virtual GraphNode *getNode(node_id id) const = 0;
    virtual void addEdge(node_id from, node_id to, edge_id id) = 0;
    virtual void removeEdge(edge_id id) = 0;
    virtual void clear() = 0;
    virtual void apply() = 0;
    virtual tt::BoundingBox boundingBox() const = 0;
    virtual void setStrategy(std::shared_ptr<LayoutStrategy> s) = 0;
    virtual std::vector<GraphNode> nodes() const = 0;
    virtual std::vector<GraphEdge> edges() const = 0;
    virtual Transform &transform() = 0;
    virtual const Transform &transform() const = 0;
};