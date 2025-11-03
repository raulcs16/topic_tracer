#pragma once

#include "layout_strategy.hpp"


class OGDFStrat : public LayoutStrategy {

public:
    explicit OGDFStrat(OGDFContext &ctx) : m_ctx(ctx) {}
    virtual ~OGDFStrat() = default;
    void apply(std::vector<GraphNode> &nodes, std::vector<GraphEdge> &edges) override;

protected:
    ogdf::LayoutModule *m_layout = nullptr;
    OGDFContext &m_ctx;
};