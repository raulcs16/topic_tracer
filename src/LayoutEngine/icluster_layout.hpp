#pragma once

#include "layout_strategy.hpp"
#include "layout_types.hpp"
#include <cstdint>
#include <string>


class IClusterLayout {
public:
    virtual GraphNode &addNode(uint32_t id) = 0;
    //silent add
    virtual void appendNode(uint32_t id) = 0;
    virtual void removeNode(uint32_t id) = 0;
    virtual void addEdge(uint32_t from, uint32_t to) = 0;
    virtual void removeEdge(uint32_t from, uint32_t to) = 0;
    virtual void clear() = 0;
    virtual void apply() = 0;
    virtual BoundingBox boundingBox() const = 0;
    virtual void setStrategy(std::shared_ptr<LayoutStrategy> s) = 0;
    virtual std::vector<GraphNode> nodes() const = 0;
    virtual std::vector<GraphEdge> edges() const = 0;
    virtual Transform &transform() = 0;
    virtual const Transform &transform() const = 0;
};