#include "ogdf_strategy.hpp"


OGDFStrategy::OGDFStrategy() {}
void OGDFStrategy::apply(std::vector<GraphNode> &nodes,
                         std::vector<GraphEdge> &edges,
                         tt::BoundingBox &bbox) {
    if (!m_layout)
        return;
    try {
        if (auto ctx = m_ctx.lock()) {
            m_layout->call(ctx->attributes);
            ctx->attributes.addNodeCenter2Bends();
            bbox.min_x = std::numeric_limits<double>::max();
            bbox.min_y = std::numeric_limits<double>::max();

            bbox.max_x -= std::numeric_limits<double>::max();
            bbox.max_y -= std::numeric_limits<double>::max();
            for (auto &node : nodes) {
                auto it = ctx->idToNode.find(node.id);
                if (it == ctx->idToNode.end())
                    continue;
                double x = ctx->attributes.x(it->second);
                double y = ctx->attributes.y(it->second);
                node.pos.x = x;
                node.pos.y = y;
                bbox.max_x = std::max(x, bbox.max_x);
                bbox.min_x = std::min(x, bbox.min_x);
                bbox.max_y = std::max(y, bbox.max_y);
                bbox.min_y = std::min(y, bbox.min_y);
            }
            for (auto &edge : edges) {
                auto src = ctx->idToNode.find(edge.from);
                auto target = ctx->idToNode.find(edge.to);
                if (src == ctx->idToNode.end() || target == ctx->idToNode.end())
                    continue;

                edge.line.start.x = ctx->attributes.x(src->second);
                edge.line.start.y = ctx->attributes.y(src->second);
                edge.line.end.x = ctx->attributes.x(target->second);
                edge.line.end.y = ctx->attributes.y(target->second);
                auto e = ctx->keyToEdge.find(edge.key);
                if (e == ctx->keyToEdge.end())
                    continue;
                edge.line.bends.clear();
                for (ogdf::DPoint &p : ctx->attributes.bends(e->second)) {
                    edge.line.bends.emplace_back(p.m_x, p.m_y);
                    bbox.max_x = std::max(bbox.max_x, p.m_x);
                    bbox.min_x = std::min(bbox.min_x, p.m_x);
                    bbox.max_y = std::max(bbox.max_y, p.m_y);
                    bbox.min_y = std::min(bbox.min_y, p.m_y);
                }
            }
            double padding = 30;
            bbox.max_x += padding;
            bbox.min_x -= padding;
            bbox.max_y += padding;
            bbox.min_y -= padding;
        }
    } catch (ogdf::AlgorithmFailureException &e) {
        std::cerr << "Algorithmn Failure: " << (int)(e.exceptionCode()) << "\n";
    } catch (...) {
        std::cerr << "unknown error\n";
    }
}
void OGDFStrategy::setContext(std::weak_ptr<OGDFContext> context) { m_ctx = context; }