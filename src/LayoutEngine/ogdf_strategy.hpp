#pragma once

#include "layout_strategy.hpp"
#include "ogdf_context.hpp"


class OGDFStrategy : public LayoutStrategy {

public:
    OGDFStrategy();
    virtual ~OGDFStrategy() = default;
    void apply(std::vector<GraphNode> &nodes,
               std::vector<GraphEdge> &edges,
               BoundingBox &bbox) override;
    void setContext(std::weak_ptr<OGDFContext> context);

protected:
    ogdf::LayoutModule *m_layout = nullptr;
    std::weak_ptr<OGDFContext> m_ctx;
};